<?php    
 /* CAT:Line chart */ 

 /* pChart library inclusions */ 
 include("../class/pData.class.php"); 
 include("../class/pDraw.class.php"); 
 include("../class/pImage.class.php"); 

mysql_connect("yourhost", "youruser", "yourpwd");
mysql_select_db("yourdb");
$query = "SELECT * FROM letture";
$result  = mysql_query($query);

while($row = mysql_fetch_array($result)) 
{
   $timestamp[] = $row["Ora"];
   $umiditaInt[] = $row["UmiditaInterna"];
   $umiditaEst[] = $row["UmiditaEsterna"];
}
$myData = new pData();
$myData->addPoints($timestamp,"Timestamp");
$myData->addPoints($umiditaInt,"UmiditaInterne");
$myData->addPoints($umiditaEst,"UmiditaEsterne");
 
$myData->setSerieWeight("UmiditaInterne",2);
$myData->setSerieWeight("UmditaEsterne",2);

 $myData->setAbscissa("Timestamp");
 $myData->setXAxisName("Time");
  //$myData->setXAxisDisplay(AXIS_FORMAT_TIME,"H:i");
  $myData->setAxisName(0,"Umidita");
 $myData->setAxisUnit(0,"%");
 /* Second Y axis will be dedicated to humidity */
$myPicture = new pImage(1000,500,$myData); 


$myPicture->drawRectangle(0,0,999,499,array("R"=>100,"G"=>30,"B"=>180));
 
/* Write the chart title */  
 $myPicture->setFontProperties(array("FontName"=>"../fonts/Forgotte.ttf","FontSize"=>11)); 
 $myPicture->drawText(150,35,"Umidità",array("FontSize"=>20,"Align"=>TEXT_ALIGN_BOTTOMMIDDLE)); 

$myPicture->setGraphArea(70,40,800,360);

 /* Draw the scale */ 
 $scaleSettings = array("XMargin"=>10,"YMargin"=>10,"Floating"=>TRUE,"GridR"=>200,"GridG"=>200,"GridB"=>200,"DrawSubTicks"=>TRUE,"CycleBackground"=>TRUE,"DrawSubTicks"=>TRUE,"DrawArrows"=>TRUE,"ArrowSize"=>6,"LabelRotation"=>90); 
 $myPicture->drawScale($scaleSettings); 


$myPicture->setFontProperties(array("FontName"=>"../fonts/Forgotte.ttf","FontSize"=>15));
$myPicture->drawLegend(390,9,array("Style"=>LEGEND_NOBORDER,"Mode"=>LEGEND_HORIZONTAL)); 
$myPicture->drawLineChart();
$myPicture->drawPlotChart(array("DisplayValues"=>TRUE,"PlotBorder"=>TRUE,"BorderSize"=>2,"Surrounding"=>-60,"BorderAlpha"=>80));
$myPicture->Stroke(); 
?>
