package neusoft.www.capcplus;

/**
 * Created by wangmian on 2018/5/23.
 * you can contact me with wangmian1994@outlook.com
 */

public class NativeHelper {
    //获取C中隐藏的AppKey
    public static  native String getAppKey();

    public static  native String getSha1(String str);

    public static  native String getSha2(String str);
}
