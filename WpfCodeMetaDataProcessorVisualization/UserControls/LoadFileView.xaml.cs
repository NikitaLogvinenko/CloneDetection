using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WpfCodeMetaDataProcessorVisualization.UserControls
{
    /// <summary>
    /// Interaction logic for LoadFileView.xaml
    /// </summary>
    public partial class LoadFileView : UserControl
    {
        public LoadFileView()
        {
           InitializeComponent();
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();

            bool? response = dlg.ShowDialog();

            if (response == true) 
            { 
                string filePath = dlg.FileName;

                MessageBox.Show(filePath);
            }
        }
    }
}
