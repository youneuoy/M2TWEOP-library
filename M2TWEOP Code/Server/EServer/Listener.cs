using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Net.Sockets;
using System.Text;
using System.Threading.Tasks;
using static EServer.Server;

namespace EServer
{
    internal class Listener : IDisposable
    {
        private TcpListener LowListener;
        private Task Listen;
        private CancellationTokenSource ListenStop = new();

        private ConcurrentDictionary<TcpClient, DateTime> Clients = new();

        //5 mb
        private byte[] RecvBuf = new byte[1024 * 1024 * 5];
        //better not set to recv buffer size,
        //so we can know if client sends more, than he allowed to
        private int RecvMaxSize = 1024 * 1024 * 5 - 100;

        //10 mb
        private int RespMaxSize = 1024 * 1024 * 10;

        MessageReceived OnMessageReceived;
        public Listener(int port, MessageReceived messageReceived)
        {
            OnMessageReceived = messageReceived;
            LowListener = new TcpListener(System.Net.IPAddress.Any, port);
            LowListener.Start();
            Listen = new Task(StartListen);
            Listen.Start();
        }

        public void Dispose()
        {
            ListenStop.Cancel();
            //wait until all operations with listener closed and resources deleted
            while (ListenStop != null)
            {
                Thread.Sleep(1);
            }
            LowListener.Stop();
        }


        private void CheckTimeOuts()
        {
            var expired = Clients.Where(client => client.Value < DateTime.Now.AddSeconds(10)).Select(client => client.Key).ToList();

            expired.ForEach(ex =>
            {
                if (Clients.Remove(ex, out var client) == false)
                {
                    return;
                }
                ex.Close();
            }
            );
        }
        private void AcceptIncomes()
        {
            //accept only one per call only, so it not take long time
            if (LowListener.Pending() == false)
            {
                return;
            }

            Clients.TryAdd(LowListener.AcceptTcpClient(), DateTime.Now);
        }
        private void ProcessMessages()
        {
            foreach (var client in Clients.Keys)
            {
                try
                {
                    if (client.Available == 0)
                    {
                        continue;
                    }

                    var stream = client.GetStream();

                    if (stream.Read(RecvBuf, 0, RecvBuf.Length) > RecvMaxSize)
                    {

                    }

                    if (OnMessageReceived(RecvBuf, out var resp) == true)
                    {
                        stream.Write(resp, 0, resp.Length);
                    }
                    Clients[client] = DateTime.Now;
                }
                catch 
                {
                    Clients.Remove(client, out var _ );
                }

            }
        }
        private void StartListen()
        {
            do
            {
                try
                {
                    CheckTimeOuts();
                    AcceptIncomes();
                    ProcessMessages();
                }
                catch (Exception)
                {
                }
                Thread.Sleep(10);
            } while (ListenStop.IsCancellationRequested == false);


            ListenStop.Dispose();
#pragma warning disable CS8625 // Cannot convert null literal to non-nullable reference type.
            ListenStop = null;
#pragma warning restore CS8625 // Cannot convert null literal to non-nullable reference type.
        }
    }
}
