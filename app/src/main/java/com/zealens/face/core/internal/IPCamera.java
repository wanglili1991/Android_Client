package com.zealens.face.core.internal;

import com.zealens.face.domain.DomainConst;

/**
 * 彩色相机录制接口
 */
public class IPCamera implements IPCameraPresenter {
    static {
        System.loadLibrary("ipcamera_sdk");
    }

    /**
     * 相机初始化函数
     *
     * @param tmpPath 临时路径
     * @param ip       相机IP地址
     * @param port     相机端口，默认：80
     * @param userName 相机登陆用户名，默认：admin
     * @param password 相机登陆密码：默认：admin
     * @param cameraArea 相机所在区域，仅支持：TennisBase.CourtArea.A , TennisBase.CourtArea.B
     * @return 初始化成功，返回 true；否则，返回 false
     * @see DomainConst#IP_CAMERA_DEFAULT_PORT
     * @see DomainConst#IP_CAMERA_DEFAULT_USERNAME
     * @see DomainConst#IP_CAMERA_DEFAULT_PASSWORD
     */
    public native boolean init(final String tmpPath, final String ip, short port, final String userName, final String password, @TennisBase.CourtArea int cameraArea);

    /**
     * @param cameraArea 相机所在区域，仅支持：TennisBase.CourtArea.A , TennisBase.CourtArea.B
     * 相机释放函数
     */
    public native void dispose(@TennisBase.CourtArea int cameraArea);

    /**
     *  开始录制录像，并保存在指定目录
     * @param savePath  录像保存路径，注意：不能使用相同路径保存录像
     * @param cameraArea 相机所在区域，仅支持：TennisBase.CourtArea.A , TennisBase.CourtArea.B
     * @return  录制成功，返回 true；否则，返回 false
     */
    public native boolean startRecord(final String savePath, @TennisBase.CourtArea int cameraArea);

    /**
     *  停止录制录像
     * @param cameraArea 相机所在区域，仅支持：TennisBase.CourtArea.A , TennisBase.CourtArea.B
     * @return  停止录制成功，返回 true；否则，返回 false
     */
    public native boolean stopRecord(@TennisBase.CourtArea int cameraArea);
}
