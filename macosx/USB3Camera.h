/*  portVideo, a cross platform camera framework
 Copyright (C) 2005-2014 Martin Kaltenbrunner <martin@tuio.org>
 
 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.
 
 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.
 
 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

//
//  USB3Camera.h
//  reacTIVision
//
//  Created by Arthur Cormack on 2015-03-18.
//
//

#ifndef reacTIVision_USB3Camera_h
#define reacTIVision_USB3Camera_h


//#import <AVFoundation/AVFoundation.h>
#include <sys/time.h>
#include <pthread.h>
//#include <gtk/gtk.h>
//#include "/usr/local/Cellar/gtk+/2.24.25/include/gtk-2.0/gtk/gtk.h"
/*
 ok ... swo here is the crux ... we want to copu the frame into the cam_buffer ... but the data that the frame is in, doesn't want to be cast into char so easily. so what are our options, to recast it? well ... maybe make it a guint8, but maybe it is something a little different ... maybe it is the way that we are trying to cast it, not what we are trying to cast it inot that is the problem here
 
 */


#include <string.h>

#include <m3api/xiApi.h>

/*//will we also need to import gobject, glib stff? gdk/gtk? gst (gstreamer)?
that was required to put the dstream into a window, and display it ... so ... can we pipe/bus directly in,or do we have to put it through the gdstreamer in order to massage it into a format that we can work with here?*/


#include <memory.h>

#include <unistd.h>
#include "../common/CameraEngine.h"
//#include <dc1394/dc1394.h>

class USB3Camera : public CameraEngine
{
public:
    USB3Camera(const char* cfg);
    ~USB3Camera();
    
    static void listDevices();
    
    bool findCamera();
    bool initCamera();
    bool startCamera();
    unsigned char* getFrame();
    bool stopCamera();
    bool stillRunning();
    bool resetCamera();
    bool closeCamera();
    
    int getCameraSettingStep(int mode);
    bool setCameraSettingAuto(int mode, bool flag);
    bool getCameraSettingAuto(int mode);
    bool setCameraSetting(int mode, int value);
    int getCameraSetting(int mode);
    int getMaxCameraSetting(int mode);
    int getMinCameraSetting(int mode);
    int getDefaultCameraSetting(int mode);
    bool setDefaultCameraSetting(int mode);
    
    void* videoDisplay(void*);
    HANDLE handle;//for some reason that i don't quite comprehend, the handle doesn't live on
    
    
    
private:
    /*
     dc1394_t *d;
    dc1394camera_list_t *list;
    dc1394camera_t *camera;
    dc1394color_coding_t coding;
     */
    //usb3 specific stuff here
    
    /* so with regards to the list
     ... we either have to find a congruent structure in xiAPI ... or ... we have to construct one ... make a struct that has the same required properties / members */
    
    
    unsigned long getcurus();
    
    XI_IMG_FORMAT prev_format = XI_RAW8;
    XI_IMG image;
    
    
    BOOLEAN acquire, quitting, render = TRUE;
    
    
    int maxcx, maxcy, roix0, roiy0, roicx, roicy;
    
    unsigned long frames;
    unsigned long prevframes;
    unsigned long lostframes;
    unsigned long curtime, prevtime;
    //pthread_t videoThread;
    
};

#endif
