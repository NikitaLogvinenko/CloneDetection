using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace WpfCodeMetaDataProcessorVisualization.ConstantsAndNotifications
{
    public static class ConfigConstants
    {
        public static readonly string _configName = "config.txt";
        public static readonly string _settingsConfigName = "settings_config.txt";

        public static readonly string _astFileFolderAdding = "_dump/";
        public static readonly string _firstDirName = "<FirstProjectDirectory>";
        public static readonly string _secondDirName = "<SecondProjectDirectory>";

        public static readonly string _fullCompareResultFolder = "<Output> \".\\results\\Result.txt\"";

        public static int _outDirPos = 0;
        public static int _firstDirPos = 2;
        public static int _secondDirPos = 3;
    }
}
