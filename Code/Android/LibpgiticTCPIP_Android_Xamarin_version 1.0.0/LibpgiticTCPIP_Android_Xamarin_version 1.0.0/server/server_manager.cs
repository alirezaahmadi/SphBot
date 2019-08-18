using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using System.Net.Sockets;
using System.Threading;
using System.Net;

//=======================================================================
//PGITIC TCP SERVER LIBRARY CLASS FOR ANDROID XAMARIN MONO # 2
//LAST UPDATE : 1394/1/23
//BY : EDWIN BABAIANS - EMAIL : edwin.babaians@gmail.com
//LICENSED : GNU PUBLIC LICENSE
//V 1.0
//=======================================================================

namespace server
{
    public class TCPEventArgs
    {
        public my_server from;
        public string message;
    }

	public class server_manager
	{
       

        public class ManagerEventArgs
        {
            public string message;
        }

        public static bool active = false;
        public static int port = 7676;
        public static List<my_server> client_list = new List<my_server>();

        private TcpListener tcpListener;
        private Thread listenThread;

        public delegate void del_update_status(string msg);

        public event TCPEventHandler get_message;
        public delegate void TCPEventHandler(TCPEventArgs args);

        public event ManagerEventHandler statues_update;
        public delegate void ManagerEventHandler(ManagerEventArgs args);

        

        public bool Init()
        {
            try
            {
                if (tcpListener != null) return false;
             
                tcpListener = new TcpListener(IPAddress.Any, port);
                listenThread = new Thread(new ThreadStart(ListenForClients));
                listenThread.Start();
                active = true;
                ManagerEventArgs mes = new ManagerEventArgs();
                mes.message = "Server Started";
                statues_update(mes);
            }
            catch
            {
                active = false;
                return false;
            }

            return true;
        }
        private void ListenForClients()
        {
            this.tcpListener.Start();

            while (active)
            {
                try
                {
                    TcpClient client = this.tcpListener.AcceptTcpClient();
                    my_server myclient = new my_server(client);
                    myclient.get_message += myclient_get_message;
                    ManagerEventArgs mes = new ManagerEventArgs();
                    mes.message = "New Client Connected";
                    if ( statues_update != null )
                    statues_update(mes);
                }
                catch (System.Exception ex)
                {
                    break;
                }
            }

            active = false;
            if ( tcpListener != null )
            tcpListener.Stop();
        }

        void myclient_get_message(TCPEventArgs args)
        {
             if ( statues_update != null )
             {
                 ManagerEventArgs mes = new ManagerEventArgs();
                 mes.message = args.message;
                 statues_update(mes);
             }   
        }
        public void kill()
        {
            try
            {
                tcpListener.Stop();
                tcpListener = null;
                active = false;
                ManagerEventArgs mes = new ManagerEventArgs();
                mes.message = "Server Stoped";
                if ( statues_update != null )
                statues_update(mes);

                for  (int i = 0 ; i < client_list.Count ; i++)
                {
                    client_list[i].Disconnect();
                }

                client_list = new List<my_server>();
            }
            catch
            {

            }
        }
        public server_manager()
        {
           
        }
	}
}