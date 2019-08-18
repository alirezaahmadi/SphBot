using System;
using Android.App;
using Android.Content;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.OS;
using Android.Content.PM;

namespace client
{
    [Activity(Label = "PGITIC", MainLauncher = true, Icon = "@drawable/icon", ScreenOrientation = ScreenOrientation.Portrait)]
    public class MainActivity : Activity
    {

        my_client client;
        TextView lst_info;

        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            // Set our view from the "main" layout resource
            SetContentView(Resource.Layout.Main);

            // Get our button from the layout resource,
            // and attach an event to it
            lst_info = FindViewById<TextView>(Resource.Id.lst_info);

            Button btn_connect = FindViewById<Button>(Resource.Id.btn_connect);
            btn_connect.Click += btn_connect_Click;
            Button btn_send = FindViewById<Button>(Resource.Id.btn_send);
            btn_send.Click += btn_send_Click;

            Button btn_disconnect = FindViewById<Button>(Resource.Id.btn_disconnect);
            btn_disconnect.Click += btn_disconnect_Click;

            client = new my_client("android");
            client.get_message += client_get_message;

        }

        void btn_disconnect_Click(object sender, EventArgs e)
        {
            client.Disconnect();
        }

        void btn_send_Click(object sender, EventArgs e)
        {
            client.write_send("Hello\n\r");
        }

        void btn_connect_Click(object sender, EventArgs e)
        {
            bool r = client.Connect();
        }

        void client_get_message(ClientEventArgs args)
        {
            RunOnUiThread(() => {
                lst_info.Text = lst_info.Text + args.message + "\n";    
            });
        }
    }
}

