<?php
// phpinfo(); // liefert Informationen zur PHP-Konfiguration
// serieller Datentransfer Pico --> Raspberry Pi, Ausgabe der Daten im Webbrowser mit diesem php-Skript (s.a. "Getting started with Raspberry Pi Pico", Kap. 4.5)
// Raspberry Pi Shell: "sudo minicom -b 115200 -o -D /dev/serial0 -C captureFileCPM.txt" zum Erzeugen der Textdatei
echo "Counts Per Minute (CPM), transmitted by Geiger Counter HW revision 3.2";
echo '<br />';
$inhalt = file_get_contents('/home/pi/captureFileCPM.txt');
$zeilen = explode("\r\n", $inhalt);
foreach ($zeilen as $zeile)
  echo $zeile.'<br />';
echo '<br />';
?>
