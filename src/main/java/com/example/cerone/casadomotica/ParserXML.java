package com.example.cerone.casadomotica;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserFactory;

import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.URL;

/**
 * Created by Cerone on 03/05/2015.
 */
public class ParserXML
{
    private String button1 = "button1";
    private String button2 = "button2";
    private String button3 = "button3";
    private String button4 = "button4";
    private String button5 = "button5";
    private String button6 = "button6";
    private String button7= "button7";
    private String button8 = "button8";
    private String button9 = "button9";
    private String button10 = "button10";
    private String button11 = "button11";
    private String button12 = "button12";
    private String button13 = "button13";
    private String button14 = "button14";

    private String tempInt = "tempInt";
    private String umidInt = "umidInt";
    private String tempEst = "tempEst";
    private String umidEst = "umidEst";


    private String urlString = null;
    private XmlPullParserFactory xmlFactoryObject;

    public volatile boolean parsingComplete = true;

    public ParserXML(String url){
        this.urlString = url;
    }

    public String getButton1() {
        return button1;
    }

    public String getUmidEst() {
        return umidEst;
    }

    public String getTempEst() {
        return tempEst;
    }

    public String getUmidInt() {
        return umidInt;
    }

    public String getTempInt() {
        return tempInt;
    }


    public String getButton12() {
        return button12;
    }

    public String getButton11() {
        return button11;
    }

    public String getButton10() {
        return button10;
    }

    public String getButton9() {
        return button9;
    }

    public String getButton8() {
        return button8;
    }

    public String getButton7() {
        return button7;
    }

    public String getButton6() {
        return button6;
    }

    public String getButton5() {
        return button5;
    }

    public String getButton4() {
        return button4;
    }

    public String getButton3() {
        return button3;
    }

    public String getButton2() {
        return button2;
    }

    public String getButton13() {
        return button13;
    }

    public String getButton14() {
        return button14;
    }

    public void parseXMLAndStoreIt(XmlPullParser myParser) {
        int event;
        String text=null;
        try {
            event = myParser.getEventType();
            while (event != XmlPullParser.END_DOCUMENT) {
                String name=myParser.getName();
                switch (event){
                    case XmlPullParser.START_TAG:
                        break;
                    case XmlPullParser.TEXT:
                        text = myParser.getText();
                        break;

                    case XmlPullParser.END_TAG:
                        if(name.equals("button1")){
                            button1 = text;
                        }
                        else if(name.equals("button2")){
                            button2 = text;// myParser.getText();
                        }
                        else if(name.equals("button3")){
                            button3 = text;//myParser.getText();
                        }
                        else if(name.equals("button4")){
                            button4 = text;//myParser.getText();
                        }
                        else if(name.equals("button5")){
                            button5 = text;//myParser.getText();
                        }
                        else if(name.equals("button6")){
                            button6 = text;//myParser.getText();
                        }
                        else if(name.equals("button7")){
                            button7 = text;//myParser.getText();
                        }
                        else if(name.equals("button8")){
                            button8 = text;//myParser.getText();
                        }
                        else if(name.equals("button9")){
                            button9 = text;//myParser.getText();
                        }
                        else if(name.equals("button10")){
                            button10 = text;//myParser.getText();
                        }
                        else if(name.equals("button11")){
                            button11 = text;//myParser.getText();
                        }
                        else if(name.equals("button12")){
                            button12 = text;//myParser.getText();
                        }
                        else if(name.equals("button13")){
                            button13 = text;
                        }
                        else if(name.equals("button14")){
                            button14 = text;
                        }
                        else if(name.equals("tempInt")){
                            tempInt = text;//myParser.getText();
                        }
                        else if(name.equals("tempEst")){
                            tempEst = text;//myParser.getText();
                        }
                        else if(name.equals("umidInt")){
                            umidInt = text;//myParser.getText();
                        }
                        else if(name.equals("umidEst")){
                            umidEst = text;//myParser.getText();
                        }
                        else{
                        }
                        break;
                }
                event = myParser.next();

            }
            parsingComplete = false;
        } catch (Exception e) {
            e.printStackTrace();
        }

    }
    public void fetchXML(){
        Thread thread = new Thread(new Runnable(){
            @Override
            public void run() {
                try {
                    URL url = new URL(urlString);
                    HttpURLConnection conn = (HttpURLConnection)
                            url.openConnection();
                    conn.setReadTimeout(10000 /* milliseconds */);
                    conn.setConnectTimeout(15000 /* milliseconds */);
                    conn.setRequestMethod("GET");
                    conn.setDoInput(true);
                    conn.connect();
                    InputStream stream = conn.getInputStream();

                    xmlFactoryObject = XmlPullParserFactory.newInstance();
                    XmlPullParser myparser = xmlFactoryObject.newPullParser();

                    myparser.setFeature(XmlPullParser.FEATURE_PROCESS_NAMESPACES
                            , false);
                    myparser.setInput(stream, null);
                    parseXMLAndStoreIt(myparser);
                    stream.close();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        });

        thread.start();


    }
}
