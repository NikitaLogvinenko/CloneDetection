cd results
del *.txt
cd ..\configs\

START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_original_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original1_original2_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_s1a_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_s1b_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_s1c_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_s2a_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_s2b_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_s2c_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_s2d_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_s3a_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_s3b_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_s3c_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_s3d_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_s3e_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_s4a_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_s4b_config.txt
START ..\..\..\..\_build\x64\Debug\CMCDApp-x64-dbg.exe original_s4c_config.txt

ECHO all files were analysed, results were saved in the results directory
PAUSE