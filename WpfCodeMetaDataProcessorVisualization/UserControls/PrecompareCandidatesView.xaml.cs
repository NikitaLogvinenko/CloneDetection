using CodeMetaData;
using FileStorageSystem.FileId;
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
    /// Interaction logic for PrecompareCandidatesView.xaml
    /// </summary>
    public partial class PrecompareCandidatesView : UserControl
    {
        FileId _selectedItem;
        public FileId SelectedItem => _selectedItem;

        public PrecompareCandidatesView()
        {
            InitializeComponent();
        }

        public event RoutedEventHandler ChooseCandidateElementClick;

        private void MouseLeftButtonDown(object sender, MouseButtonEventArgs e)
        {
            ListView view = sender as ListView;

            if (view != null)
            {
                _selectedItem = (FileId)view.SelectedItem;
            }

            ChooseCandidateElementClick?.Invoke(this, e);
        }
    }
}
