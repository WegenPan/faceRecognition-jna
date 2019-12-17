package com.xxx.xxx;  //修改成自己的包名

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.IntByReference;
import java.io.File;
import java.awt.image.BufferedImage;
import java.awt.Color;
import javax.imageio.ImageIO;
import java.io.IOException;
import java.lang.reflect.Method;

public class jnaTest {
    public interface Clibrary extends Library {

        Clibrary INSTANTCE = (Clibrary) Native.loadLibrary("e:/jna/build/Face",
                Clibrary.class);

        boolean FaceModelInit(String weightPath);
        Pointer FaceDetect(byte[] imageDate_, int imageWidth, int imageHeight, int imageChannel);
        String FaceFeature(byte[] imageDate_, int w, int h);
        void Test();
    }

    public static void main(String[] args) {
        Clibrary.INSTANTCE.Test();
        System.out.println(jnaTest.class.getResource("face/test.json"));
        System.out.println(jnaTest.class.getResource("det1.bin"));
        try{
            File file=new File("E:/jna/build/wegen.jpg");
            BufferedImage bufIma=ImageIO.read(file);//
            int width=bufIma.getWidth();
            int height=bufIma.getHeight();
            byte[] imgRGBA = getPixels(bufIma, 0, 0, width, height);
            boolean inital_ = Clibrary.INSTANTCE.FaceModelInit("E:/jna/build/weights");
            if(inital_){
                Pointer pInt;
                pInt = Clibrary.INSTANTCE.FaceDetect(imgRGBA, width, height, 4);
                int faceNum = pInt.getInt(0);
                System.out.println("face num:"+faceNum);
                if(faceNum==1){
                    int[] arr = pInt.getIntArray(0, 1+faceNum*14);
                    int left = arr[1], top = arr[2], w = arr[3]-arr[1], h = arr[4]-arr[2];
                    byte[] faceRGBA = getPixels(bufIma, left, top, w, h);
                    String feastr = Clibrary.INSTANTCE.FaceFeature(faceRGBA, w, h);
                    System.out.println(feastr);
                }
            }

        } catch(Exception e){
            e.printStackTrace();
        }

    }

    private static byte[] getPixels(BufferedImage image, int left, int top, int width, int height) {
        byte[] result = new byte[height*width*4];
        for (int y = top, j=0; y < top+height; y++,j++) {
            for (int x = left, i=0; x < left+width; x++,i++) {
                Color c = new Color(image.getRGB(x, y), true);
                result[j*width*4 + i*4] = (byte)c.getRed();
                result[j*width*4 + i*4 + 1] = (byte)c.getGreen();
                result[j*width*4 + i*4 + 2] = (byte)c.getBlue();
                result[j*width*4 + i*4 + 3] = (byte)c.getAlpha();
            }
        }
        return result;
    }

}
