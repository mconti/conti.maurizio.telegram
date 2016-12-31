using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Runtime.InteropServices.WindowsRuntime;
using Telegram.Bot;
using Telegram.Bot.Types.Enums;
using Windows.Foundation;
using Windows.Foundation.Collections;
using Windows.UI.Xaml;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Controls.Primitives;
using Windows.UI.Xaml.Data;
using Windows.UI.Xaml.Input;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Navigation;

// Il modello di elemento per la pagina vuota è documentato all'indirizzo http://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x410

namespace conti.maurizio.telegram
{
    /// <summary>
    /// Pagina vuota che può essere usata autonomamente oppure per l'esplorazione all'interno di un frame.
    /// </summary>
    public sealed partial class MainPage : Page
    {
        // Libreria utilizzata telegram.bot
        // https://github.com/MrRoundRobin/telegram.bot
        DispatcherTimer timer = new DispatcherTimer();
        int offset = 0;
        bool stato = false;

        public MainPage()
        {
            this.InitializeComponent();

            timer.Interval = TimeSpan.FromMilliseconds(300);
            timer.Tick += Timer_Tick;
            timer.Start();
        }

        private async void Timer_Tick(object sender, object e)
        {
            timer.Stop();
            try
            {
                //var me = await Bot.GetMeAsync();
                //var result = await Bot.SendTextMessageAsync("mconti", "Hello World!");
                //log.Items.Add("me.FirstName: " + me.FirstName);

                var updates = await Bot.GetUpdatesAsync(offset);

                foreach (var update in updates)
                {
                    switch (update.Type)
                    {
                        case UpdateType.MessageUpdate:
                            var message = update.Message;

                            switch (message.Type)
                            {
                                case MessageType.TextMessage:

                                    switch (message.Text)
                                    {
                                        case "/toggle":
                                            stato = ToggleLED(stato);

                                            if( stato)
                                                await Bot.SendTextMessageAsync(message.Chat.Id, "Acceso", replyToMessageId: message.MessageId);
                                            else
                                                await Bot.SendTextMessageAsync(message.Chat.Id, "Spento", replyToMessageId: message.MessageId);
                                            break;

                                        case "/status":
                                            if (stato)
                                                await Bot.SendTextMessageAsync(message.Chat.Id, "Acceso", replyToMessageId: message.MessageId);
                                            else
                                                await Bot.SendTextMessageAsync(message.Chat.Id, "Spento", replyToMessageId: message.MessageId);
                                            break;

                                        default:
                                            await Bot.SendTextMessageAsync(message.Chat.Id, $"{message.Text} ??", replyToMessageId: message.MessageId);
                                            break;
                                    }
                                    break;
                            }
                            break;
                    }

                    offset = update.Id + 1;
                }
            }
            catch (Exception err)
            {
                log.Items.Add("Err: " + err.Message);
            }

            timer.Start();
        }

        private bool ToggleLED( bool stato )
        {
            if (stato)
                btn.Background = new SolidColorBrush(Windows.UI.Color.FromArgb(255, 255, 0, 0));
            else
                btn.Background = new SolidColorBrush(Windows.UI.Color.FromArgb(255, 0, 255, 0));

            return !stato;
        }

    }
}
