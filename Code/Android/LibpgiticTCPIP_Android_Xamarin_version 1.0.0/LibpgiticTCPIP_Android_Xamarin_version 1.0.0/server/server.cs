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

//=======================================================================
//PGITIC TCP SERVER LIBRARY CLASS FOR ANDROID XAMARIN MONO # 2
//LAST UPDATE : 1394/1/23
//BY : EDWIN BABAIANS - EMAIL : edwin.babaians@gmail.com
//LICENSED : GNU PUBLIC LICENSE
//V 1.0
//=======================================================================

namespace server
{
    public class my_server
	{
        public string name;
        TcpClient client;

        public event TCPEventHandler get_message;
        public delegate void TCPEventHandler(TCPEventArgs args);

        

        public void Disconnect()
        {
            if (client != null)
            {
                client.Close();
                client = null;
            }
        }

        public my_server(TcpClient tcpclient)
        {
            client = tcpclient;
            server_manager.client_list.Add(this);
            Thread clientThread = new Thread(new ThreadStart(HandleClientComm));
            clientThread.Start();
        }

        void read_recive(string msg)
        {
             //MSG IS MESSAGE FROM THAT CLIENT - PROTOCOL ANALYSIS HERE :
             //SHOW CLIENT MESSAGE IN INFO LISTVIEW
            TCPEventArgs mes = new TCPEventArgs();
            mes.message = msg;
            mes.from = this;
            if ( get_message != null )
            get_message(mes);
             
        }

        public void write_send(string msg)
        {
            try
            {
                NetworkStream clientStream = client.GetStream();
                byte[] buffer = Encoding.UTF8.GetBytes(msg);
                clientStream.Write(buffer, 0, buffer.Length);
                clientStream.Flush();
            }
            catch
            {
                try
                {
                    client.Close();
                }
                catch
                {

                }
                client = null;
            }
        }

        private void HandleClientComm()
        {
            //GET NETWORK STREAM
            NetworkStream clientStream = client.GetStream();
            byte[] message = new byte[4096];
            int bytesRead;

            while (server_manager.active)
            {
                bytesRead = 0;

                try
                {
                    //BLOCKED FOR NEW MESSAGE
                    bytesRead = clientStream.Read(message, 0, 4096);
                }
                catch
                {
                    //ERROR OR DISCONNECT
                    break;
                }

                if (bytesRead == 0)
                {
                    //ERROR OR DISCONNECT
                    break;
                }

                //READ DONE - CONVERT TO UTF8 STRING
                string read_str = Encoding.UTF8.GetString(message, 0, bytesRead);
                read_recive(read_str);     
            }

            TCPEventArgs mes = new TCPEventArgs();
            mes.from = this;
            mes.message = "Disconned";
            if (get_message != null)
                get_message(mes);

            server_manager.client_list.Remove(this);
           

            client.Close();
            client = null;
        }
	}
}