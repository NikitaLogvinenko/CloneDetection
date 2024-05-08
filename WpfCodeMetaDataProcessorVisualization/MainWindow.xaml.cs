using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;

namespace WpfCodeMetaDataProcessorVisualization
{
  
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {
        private string _initFileStorageName = "System.txt";
        public MainWindow()
        {
            InitializeComponent();

            InitSystem();
        }

        private async void InitSystem()
        {
            var viewModel = await FileStorageSytemView.CreateAsync(new System.IO.FileInfo(_initFileStorageName));
            this.DataContext = viewModel;
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            MessageBox.Show("Add New File");
        }

        private void LoadFileView_Loaded(object sender, RoutedEventArgs e)
        {

        }

    }
}