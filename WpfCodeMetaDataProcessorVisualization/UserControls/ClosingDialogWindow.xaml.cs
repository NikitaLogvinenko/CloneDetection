using System.Windows;

namespace WpfCodeMetaDataProcessorVisualization.UserControls
{
    /// <summary>
    /// Interaction logic for ClosingDialogWindow.xaml
    /// </summary>
    public partial class ClosingDialogWindow : Window
    {
        public ClosingDialogWindow()
        {
            InitializeComponent();
        }
        private void Accept_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
        }
    }
}
