using System.Collections.Concurrent;
using System.Net.Sockets;

namespace EServer
{
    public class Server
    {
        private ConcurrentDictionary<int, Listener> Listeners = new ConcurrentDictionary<int, Listener>();


        public delegate bool MessageReceived(byte[] message, out byte[] response);

        MessageReceived OnMessageReceived;

        public Server(MessageReceived messageReceived)
        {
            OnMessageReceived = messageReceived;
        }
        public void Run(int port)
        {
            Listeners.TryAdd(port, new Listener(port, OnMessageReceived));
        }
        public void Run(IEnumerable<int> ports)
        {
            foreach (int port in ports)
            {
                Run(port);
            }
        }

        public void Stop(int port)
        {
            if(Listeners.Remove(port, out var Listener)==false)
            {
                return;
            }

            Listener.Dispose();
        }
        public void Stop(IEnumerable<int> ports)
        {
            foreach (int port in ports)
            {
                Stop(port);
            }
        }
        public void Stop()
        {
           var ports = Listeners.Select(list => list.Key);

           Stop(ports);
        }
    }
}