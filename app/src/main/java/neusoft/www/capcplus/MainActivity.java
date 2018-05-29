package neusoft.www.capcplus;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    NativeHelper helper;

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("cap-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        String str = "{\"SERVICE_ID\":\"17615040413\",\"PRODUCT_NAME\":\"优酷免流量包\",\"PRODUCT_PRICE\":\"6元/月\",\"CLIENT_ID\":\"YfDJIu0k1K3DuDTSuGhb\",\"CLIENT\":\"1\",\"VERSION\":\"1.0.0\",\"CLIENT_SECRET\":\"12cb7a4c095b48afb21db3fbe0561415\",\"AUTH_NO\":\"152023151744232\"}";
        String  result = NativeHelper.getSha1(str);
        Toast.makeText(this,result,Toast.LENGTH_LONG).show();
    }

}
