using System;
using System.Collections.Generic;
using System.IO;
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
using System.Windows.Shapes;

namespace WpfCodeMetaDataProcessorVisualization.UserControls
{
    /// <summary>
    /// Interaction logic for Window1.xaml
    /// </summary>
    public partial class CompareResultWindow : Window
    {
        string filePath = "C:\\Users\\User\\Project\\CloneDetection\\WpfCodeMetaDataProcessorVisualization\\bin\\Debug\\net8.0-windows\\results\\Result.txt";
        public CompareResultWindow()
        {
            InitializeComponent();

            try
            {
                if (File.Exists(filePath))
                {
                    string fileContent = File.ReadAllText(filePath);
                    txtFileContent.Text = fileContent;
                }
                else
                {
                    txtFileContent.Text = "File not found.";
                }
            }
            catch (Exception ex)
            {
                txtFileContent.Text = "Error reading file: " + ex.Message;
            }
        }

        private void Accept_Click(object sender, RoutedEventArgs e)
        {
            DialogResult = true;
        }
    }
}
