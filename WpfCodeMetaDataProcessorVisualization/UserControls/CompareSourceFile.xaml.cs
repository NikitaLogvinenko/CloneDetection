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
    /// Interaction logic for CompareSourceFile.xaml
    /// </summary>
    public partial class CompareSourceFile : UserControl
    {
        public CompareSourceFile()
        {
            InitializeComponent();
        }

        public event RoutedEventHandler ButtonCompareFilesClick;

        private void CompareButtonClick(object sender, RoutedEventArgs e)
        {
            ButtonCompareFilesClick?.Invoke(this, e);
        }
    }
}
