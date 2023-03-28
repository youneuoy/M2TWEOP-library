

using EServer;
namespace M2TWEOPServer
{
    internal class MessagesHandler
    {
        private Server Server;
        public MessagesHandler()
        {
            Server = new Server(MessageReceived);
        }
        public void Run()
        {
            Server.Run(51742);
        }
        private bool MessageReceived(byte[] message, out byte[] response)
        {
            response = message.ToArray();
            return false;
        }
    }
}
