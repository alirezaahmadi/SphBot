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
using System.Net;
using System.Net.NetworkInformation;

//=======================================================================
//PGITIC TCP SERVER LIBRARY CLASS FOR ANDROID XAMARIN MONO # 2
//LAST UPDATE : 1394/1/23
//BY : EDWIN BABAIANS - EMAIL : edwin.babaians@gmail.com
//LICENSED : GNU PUBLIC LICENSE
//V 1.0
//=======================================================================


namespace client
{
    public class ClientEventArgs
    {
        public string message;
    }
    public class my_client
    {
        public static int port = 3000;
        public static string ip = "192.168.4.1";
        public static bool active = false;
        public string name;

        public TcpClient client;
        System.Timers.Timer time_out_timer;

        public event TCPEventHandler get_message;
        public delegate void TCPEventHandler(ClientEventArgs args);

        public void Disconnect()
        {
            if (client != null && active)
            {
                ClientEventArgs mes = new ClientEventArgs();
                mes.message = "Disconnected";
                if (get_message != null)
                    get_message(mes);

                client.Close();
                client = null;
                active = false;  
            } 
        }
        public bool Connect()
        {
            if (active) return true;

            try
            {
                time_out_timer = new System.Timers.Timer();
                time_out_timer.Interval = 4000;
                time_out_timer.Elapsed += time_out_timer_Elapsed;

                client = new TcpClient();
                IPEndPoint serverEndPoint = new IPEndPoint(IPAddress.Parse(ip), port);

                active = false;
                time_out_timer.Enabled = true;
                time_out_timer.Start();

                client.Connect(serverEndPoint);

                active = true;

                System.Threading.Thread clientThread = new System.Threading.Thread(new System.Threading.ThreadStart(HandleClientComm));
                clientThread.Start();

                ClientEventArgs mes = new ClientEventArgs();
                mes.message = "Connected";
                if (get_message != null)
                    get_message(mes);

                return true;

            }
            catch (System.Exception ex)
            {
                ClientEventArgs mes = new ClientEventArgs();
                mes.message = "Server is not active";
                if (get_message != null)
                    get_message(mes);

                client = null;
                name = "";
                active = false;
            }

            return false;
        }

        public my_client(string name)
        {
            this.name = name;
        }

        void time_out_timer_Elapsed(object sender, System.Timers.ElapsedEventArgs e)
        {
            //time out
            if (active == false)
            {
                client.Close();
                client = null;
                name = "";
                active = false;
                GC.WaitForFullGCComplete();

                ClientEventArgs mes = new ClientEventArgs();
                mes.message = "server is not active";
                if (get_message != null)
                    get_message(mes);
            }

            time_out_timer.Stop();
            time_out_timer.Enabled = false;
            time_out_timer = null;
        }
      
        public void write_send(string msg)
        {
            if (client != null && active)
            {
                NetworkStream clientStream = client.GetStream();
               // byte[] buffer = Encoding.UTF8.GetBytes(msg);
                byte[] buffer = new byte[10];
                buffer[0] = 255;
                buffer[1] = 100;
                buffer[5] = 23;
                buffer[9] = 255;
                clientStream.Write(buffer, 0, buffer.Length);
                clientStream.Flush();


            }
        }

        void read_process(string msg)
        {
            //PROTOCOL ANALISYS
            ClientEventArgs mes = new ClientEventArgs();
            mes.message = msg;
            if (get_message != null)
                get_message(mes);

        }
        private void HandleClientComm()
        {
                TcpClient tcpClient = client;
                NetworkStream clientStream = tcpClient.GetStream();

                byte[] message = new byte[4096];
                int bytesRead;

                while (active)
                {
                    bytesRead = 0;

                    try
                    {
                        bytesRead = clientStream.Read(message, 0, 4096);
                    }
                    catch
                    {
                      
                        break;
                    }

                    if (bytesRead == 0)
                    {
                        break;
                    }

                    string read_str = Encoding.UTF8.GetString(message, 0, bytesRead );
                    read_process(read_str);
                }

                if (tcpClient != null)
                {
                    tcpClient.Close();
                    tcpClient = null;
                }

                ClientEventArgs mes = new ClientEventArgs();
                mes.message = "Disconnected";
                if (get_message != null)
                    get_message(mes);

                active = false;  
        }
    }
}