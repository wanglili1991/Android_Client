package com.zealens.face.core.internal;

import com.zealens.face.domain.DomainConst;

/**
 * Colorful IPCamera record interface
 */
public interface IPCameraPresenter {
    /**
     * Camera initialize
     *
     * @param ip       Ip Address
     * @param port     port, defaultValue = 0
     * @param userName userName, defaultValue = admin
     * @param password password, defaultValue = admin
     * @param cameraArea cameraArea, only support TennisBase.CourtArea.A , TennisBase.CourtArea.B
     * @return true for success
     * @see DomainConst#IP_CAMERA_DEFAULT_PORT
     * @see DomainConst#IP_CAMERA_DEFAULT_USERNAME
     * @see DomainConst#IP_CAMERA_DEFAULT_PASSWORD
     */
    boolean init(final String tmpPath, final String ip, short port, final String userName, final String password, @TennisBase.CourtArea int cameraArea);

    /**
     *
     * @param cameraArea cameraArea, only support TennisBase.CourtArea.A , TennisBase.CourtArea.B
     */
    void dispose(@TennisBase.CourtArea int cameraArea);


    /**
     * start to record, store the video in the assigned path
     *
     * @param savePath path to save the record video
     * @param cameraArea cameraArea, only support TennisBase.CourtArea.A , TennisBase.CourtArea.B
     * @return true for success
     */
    boolean startRecord(final String savePath, @TennisBase.CourtArea int cameraArea);

    /**
     * @param cameraArea cameraArea, only support TennisBase.CourtArea.A , TennisBase.CourtArea.B
     * @return true for success
     */
    boolean stopRecord(@TennisBase.CourtArea int cameraArea);
}
