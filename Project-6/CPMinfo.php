<?php
// phpinfo();
echo "Counts Per Minute (CPM), transmitted by Geiger Counter HW revision 3.2";
echo '<br />';
$inhalt = file_get_contents('/home/pi/captureFileCPM.txt');
$zeilen = explode("\r\n", $inhalt);
foreach ($zeilen as $zeile)
  echo $zeile.'<br />';
echo '<br />';
?>