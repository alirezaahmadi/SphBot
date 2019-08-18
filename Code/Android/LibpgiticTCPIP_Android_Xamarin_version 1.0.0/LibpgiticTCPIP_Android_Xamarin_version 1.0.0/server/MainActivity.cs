using System;
using Android.App;
using Android.Content;
using Android.Runtime;
using Android.Views;
using Android.Widget;
using Android.OS;
using Android.Content.PM;

namespace server
{
    [Activity(Label = "PGITIC", MainLauncher = true, Icon = "@drawable/icon", ScreenOrientation = ScreenOrientation.Portrait)]
    public class MainActivity : Activity
    {
        server_manager manager;
        TextView lst_info;
        protected override void OnCreate(Bundle bundle)
        {
            base.OnCreate(bundle);

            SetContentView(Resource.Layout.Main);

            Button btn_start = FindViewById<Button>(Resource.Id.btn_start);
            Button btn_stop = FindViewById<Button>(Resource.Id.btn_stop);
            lst_info = FindViewById<TextView>(Resource.Id.lst_info);

            btn_start.Click += btn_start_Click;
            btn_stop.Click += btn_stop_Click;

            manager = new server_manager();
            manager.get_message += manager_get_message;
            manager.statues_update += manager_statues_update;
            manager.Init();
        }

        void manager_statues_update(server_manager.ManagerEventArgs args)
        {
            RunOnUiThread(() =>
            {
                lst_info.Text = lst_info.Text + args.message + "\n";    
            });
          
        }

        void manager_get_message(TCPEventArgs args)
        {
            RunOnUiThread(() =>
            {
                lst_info.Text = lst_info.Text + "GET : " + args.message + "\n";
            });
        }

        
        void btn_stop_Click(object sender, EventArgs e)
        {
            //stop the server
            manager.kill();
        }

        void btn_start_Click(object sender, EventArgs e)
        {
           //start the server
            manager.Init();
        }

        
    }
}

