package com.zealens.face;

import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.zealens.face.core.internal.IPCamera;
import com.zealens.face.core.internal.TennisBase;

import java.io.File;

/**
 * Created by Songkang on 4/1/17.
 * Email:songkang@tiantianld.com
 * Tips: This Class File for Android developer use Ipcamera SDK demo.
 */

public class CameraDemo extends Activity {
    private static final String TAG = CameraDemo.class.getSimpleName();

    private static final String ip="192.168.1.198";
    private static final short port=0;
    private static final String username="admin";
    private static final String password="123456";

    private IPCamera mHandler;

    public static boolean createFile(String strPath, String strFile) {
        File path = new File(strPath);
        if (!path.exists()) {
            try {
                if (!path.mkdirs()) {
//                    ToolKits.writeLog("App can't create path " + strPath);
                    return false;
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }

        File file = new File(strPath + strFile);
        if (file.exists()) {
            file.delete();
        }

        try {
            if (!file.createNewFile()) {
//                ToolKits.writeLog("App can't create file " + strPath + strFile);
                return false;
            }
        } catch (Exception e) {
            e.printStackTrace();
        }

        return true;
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.ipcamera_demo);
        mHandler = new IPCamera();

        File file = getFilesDir();
        final String tmpPath = file.getPath() + "/tmp/";

        if (!mHandler.init(tmpPath, ip, port, username, password, TennisBase.CourtArea.A))
            return;

        Button startButton = (Button)findViewById(R.id.start);
        startButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String path = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator + "myvideo" + File.separator;
                if (!createFile(path, "demofile"))
                    return;

                if (!mHandler.startRecord(path + "demofile", TennisBase.CourtArea.A))
                    return;

                Log.e(TAG, "Success to start Record");
            }
        });

        Button stopButton = (Button)findViewById(R.id.stop);
        stopButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                mHandler.stopRecord(TennisBase.CourtArea.A);
            }
        });
    }
}
