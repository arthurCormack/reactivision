
/*  portVideo, a cross platform camera framework
 Copyright (C) 2005-2014 Martin Kaltenbrunner <martin@tuio.org>
 
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 2 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

//
//  USB3Camera.cpp
//  reacTIVision
//
//  Created by Arthur Cormack on 2015-03-18.
//
//

#include "USB3Camera.h"
#include <stdio.h>
#include <pthread.h>

USB3Camera::USB3Camera(const char* cfg) : CameraEngine(cfg)
{
    //we can come back, and define any default values
    //some variables are defined by header files included that are included by USB3Camera.h, like CameraEngine
    
    cameraID = -1;
    cam_buffer = NULL;
    
    running=false;
    timeout= 5000;
    lost_frames=0;
   
    frames = 0;
    prevframes = 0;
    lostframes = 0;
    
    //curtime, prevtime;
    
    
}

USB3Camera::~USB3Camera()
{
    if (cam_buffer!=NULL) delete []cam_buffer;
}



void USB3Camera::listDevices() {
    
    
    //we don't actually need this ... but we might as well, for diagnostic purposes
   
    //image.size = sizeof(XI_IMG);
    //image.bp = NULL;
    //image.bp_size = 0;
    //
    //if(xiStartAcquisition(handle) != XI_OK)
    //goto exit;
    
    /*
    dc1394_t *d = dc1394_new();
    dc1394camera_list_t *list;
    
    float framerate_table[] = { 1.875f,3.75f,7.5f,15.0f,30.0f,60.0f,120.0f,240.0f };
    int width_table[] = {160,320,640,640,640,640,640,800,800,800,1024,1024,1024,800,1024,1280,1280,1280,1600,1600,1600,1280,1600 };
    int height_table[] = {120,240,480,480,480,480,480,600,600,600,768,768,768,600,768,960,960,960,1200,1200,1200,960,1200 };
    std::string codec_table[] = {"MONO8","YUV411","YUV422","YUV444","RGB8","MONO16","RGB16","MONO16S","RGB16S","RAW8","RAW16"};
    
    if (dc1394_camera_enumerate (d, &list) != DC1394_SUCCESS) {
        //fprintf (stderr, "failed to enumerate cameras\n");
        fprintf (stderr, "no DC1394 cameras found\n");
        dc1394_free(d);
        return;
    }
    
    if (list->num == 0) {
        fprintf (stderr, "no DC1394 cameras found\n");
        dc1394_free(d);
        return;
    } else if (list->num == 1) printf("1 DC1394 camera found:\n");
    else printf("%d DC1394 cameras found:\n", list->num);
    
    for (int i=0;i<list->num;i++) {
        dc1394camera_t *camera = dc1394_camera_new (d, list->ids[i].guid);
        if (!camera) continue;
        printf("\t%d: %s\n", i, camera->model);
        
        dc1394video_modes_t video_modes;
        
        if (dc1394_video_get_supported_modes(camera,&video_modes)!=DC1394_SUCCESS) {
            printf("\t\tunable to list DC1394 modes\n");
            dc1394_camera_free(camera);
            continue;
        }
        
        for (int i=0;i<video_modes.num;i++) {
            
            if(video_modes.modes[i]>=DC1394_VIDEO_MODE_FORMAT7_0) {
                dc1394color_coding_t coding;
                dc1394_get_color_coding_from_video_mode(camera,video_modes.modes[i], &coding);
                std::cout << "\t\tformat: FORMAT7_" << video_modes.modes[i]-DC1394_VIDEO_MODE_FORMAT7_0 << " "<< codec_table[coding-DC1394_COLOR_CODING_MIN] << std::endl;
                
                uint32_t F7_xmax, F7_ymax;
                dc1394_format7_get_max_image_size(camera, video_modes.modes[i], &F7_xmax, &F7_ymax);
                printf("\t\t\t%dx%d max\n",F7_xmax,F7_ymax);
                
            } else {
                
                dc1394color_coding_t coding;
                dc1394_get_color_coding_from_video_mode(camera,video_modes.modes[i], &coding);
                std::cout << "\t\tformat: " << codec_table[coding-DC1394_COLOR_CODING_MIN] << std::endl;
                
                unsigned int width,height;
                dc1394_get_image_size_from_video_mode(camera, video_modes.modes[i], &width, &height);
                int cam_width = width_table[video_modes.modes[i]-DC1394_VIDEO_MODE_MIN];
                int cam_height = height_table[video_modes.modes[i]-DC1394_VIDEO_MODE_MIN];
                printf("\t\t\t%dx%d ",cam_width,cam_height);
                
                dc1394framerates_t framerates;
                dc1394_video_get_supported_framerates(camera,video_modes.modes[i],&framerates);
                for (int j=0;j<framerates.num;j++) {
                    float fps = framerate_table[framerates.framerates[j]-DC1394_FRAMERATE_MIN];
                    if(int(fps)==fps) printf("%d|",int(fps));
                    else printf("%'.1f|",fps);
                } printf("\b fps\n");
            }
        }
        
        
        dc1394_camera_free(camera);
    }
    
    dc1394_free(d);
    dc1394_camera_free_list (list);
     */
    //return a list of cameras
    
    DWORD tmp;
    xiGetNumberDevices(&tmp);//rescan available devices. It is needed to be called before any other function of API is called by application.
    DWORD nIndex = 0;
    HANDLE handle = INVALID_HANDLE_VALUE;
    
    //how do we present some deails about the camera?
    /*
        what are we going to do about the problem of there being two competing Classes: USB3Camera, and AVFoundationCamera? Is it that reacTIVision can use both DC1394 or AVFOundationCamera? Or is it that it chooses one or the other, depending on the OS version that is in play?
            I think that the AVFOundation camera, is like a new and improved version of the lib1394, and can do al that it is able to do, like seeing lib1394 cameras. If that is the case, then, h'mm.
     Is it possible to have two Classes, and choose to use both of them?
     to invoke the USB3Camera, if no AVFOundation Cameras are found?
     
    */
    //it is quite possible that the sample code that was shipped with the XIMEA camera, is circa OSX10.7
    
    if (tmp == 0 || true) {
        fprintf (stderr, "no USB3 cameras found\n");
        //
    } else {
        if(xiOpenDevice(nIndex, &handle) != XI_OK) {
            //printf("Couldn't setup camera!\n");
            //acquire = FALSE;
            //return TRUE;
        } else {
            //what can we find out about the device?
            //char* someDeviceName;
            //xiGetParamString(handle, XI_PRM_DEVICE_NAME, someDeviceName);//xiGetParamString does not seem to exist in the API
            //ok ... so some of the stuff that is in the documentation, may not actually be available here. some things are ... so that is promising ...
            //so, we stick to things that are in the header file of the libary, and in the example that we have
            
            /*unsigned int model_id;
            xiGetParamInt(handle, XI_PRM_DEVICE_MODEL_ID, &model_id);
            
            printf("\tCamera Model: %d\n", model_id);
            // show serial number of this camera
            unsigned int sn=0;
            
            xiGetParamInt(handle, XI_PRM_DEVICE_SN, &sn);
            printf("Serial number of the camera is: %08x\n",sn);*/
            
            //DWORD DevId = ;
            //char* prm;
            DWORD value_size = 0;
            char* someThing = NULL;
            
            xiGetDeviceInfoString(nIndex, XI_PRM_DEVICE_SN, someThing, value_size);
            printf("\tCamera SN: %s\n", someThing);
            
            xiGetDeviceInfoString(nIndex, XI_PRM_DEVICE_NAME, someThing, value_size);
            printf("\tCamera Name: %s\n", someThing);
            
            
        }
    }
    
    
}
unsigned long getcurus() {
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000000 + now.tv_usec;
}

bool USB3Camera::findCamera() {
    
    /*
    d = dc1394_new();
    
    if (dc1394_camera_enumerate (d, &list) != DC1394_SUCCESS) {
        //fprintf (stderr, "failed to enumerate cameras\n");
        fprintf (stderr, "no DC1394 cameras found\n");
        dc1394_free(d);
        return false;
    }
    
    if (list->num == 0) {
        fprintf (stderr, "no DC1394 cameras found\n");
        dc1394_free(d);
        return false;
    } else if (list->num == 1) printf("1 DC1394 camera found\n");
    else printf("%d DC1394 camera found\n", list->num);
    
    readSettings();
    cameraID = config.device;
    if (cameraID<0) cameraID=0;
    
    if (cameraID>=list->num) {
        dc1394_free(d);
        return false;
    }
     
    */
    
    DWORD tmp;
   
    DWORD nIndex = 0;
    HANDLE handle = INVALID_HANDLE_VALUE;//handle stays alive outside of this
    
    
    xiGetNumberDevices(&tmp); //rescan available devices
    if (tmp == 0) {
        fprintf (stderr, "no USB3Camera cameras found\n");
        return false;
    } else {
        if(xiOpenDevice(nIndex, &handle) != XI_OK) {
            fprintf (stderr, "could not open device\n");
        } else {
            readSettings();//h'mm
            
        }
    }
    
    return true;

}

bool USB3Camera::initCamera() {
    
    
    DWORD tmp;
    xiGetNumberDevices(&tmp);//rescan available devices. It is needed to be called before any other function of API is called by application.
    DWORD nIndex = 0;
    HANDLE handle = INVALID_HANDLE_VALUE;
    
    if(xiOpenDevice(nIndex, &handle) != XI_OK) {
        printf("Couldn't setup camera!\n");
        //acquire = FALSE;
        //return TRUE;
    } else {
        printf("InitCamera!\n");
    }
    
    
    image.size = sizeof(XI_IMG);
    image.bp = NULL;
    image.bp_size = 0;
    
    /*
    //if (cameraID < 0) return false;
    
    camera = dc1394_camera_new (d, list->ids[cameraID].guid);
    if (!camera) {
        fprintf (stderr, "Failed to initialize Firewire camera\n");
        return false;
    }
    dc1394_camera_free_list (list);
    
    sprintf(cameraName,"%s", camera->model);
    
    dc1394framerates_t framerates;
    dc1394video_modes_t video_modes;
    dc1394framerate_t framerate;
    dc1394video_mode_t video_mode = DC1394_VIDEO_MODE_FORMAT7_0;
    
    dc1394color_coding_t preferred_coding = DC1394_COLOR_CODING_MONO8;
    if(colour) preferred_coding = DC1394_COLOR_CODING_RGB8;
    dc1394color_coding_t YUV411_coding = DC1394_COLOR_CODING_YUV411;
    dc1394color_coding_t YUV422_coding = DC1394_COLOR_CODING_YUV422;
    
    float framerate_table[] = { 1.875f,3.75f,7.5f,15.0f,30.0f,60.0f,120.0f,240.0f };
    int width_table[] = {160,320,640,640,640,640,640,800,800,800,1024,1024,1024,800,1024,1280,1280,1280,1600,1600,1600,1280,1600 };
    int height_table[] = {120,240,480,480,480,480,480,600,600,600,768,768,768,600,768,960,960,960,1200,1200,1200,960,1200 };
    
    bool force_FW800 = false;
    bool force_FW400 = false;
    
    if (std::string(camera->model).find("Firefly MV FFMV")!=std::string::npos) force_FW800 = true;
    if (std::string(camera->model).find("Chameleon")!=std::string::npos) force_FW400 = true;
    
    if ( ((!force_FW400) && (camera->bmode_capable==DC1394_TRUE)) || (force_FW800))  {
        // set operation mode legacy or 1394B
        dc1394_video_set_operation_mode(camera, DC1394_OPERATION_MODE_1394B);
        dc1394operation_mode_t operation_mode;
        dc1394_video_get_operation_mode(camera, &operation_mode);
        
        if (operation_mode == DC1394_OPERATION_MODE_1394B) {
            dc1394_video_set_iso_speed(camera, DC1394_ISO_SPEED_800);
        } else {
            dc1394_video_set_iso_speed(camera, DC1394_ISO_SPEED_400);
        }
    } else {
        dc1394_video_set_operation_mode(camera, DC1394_OPERATION_MODE_LEGACY);
        dc1394_video_set_iso_speed(camera, DC1394_ISO_SPEED_400);
    }
    
    if (config.frame) {
        
        dc1394format7mode_t f7_mode;
        if ( dc1394_format7_get_mode_info(camera,video_mode, &f7_mode) !=DC1394_SUCCESS) {
            std::cerr << "FORMAT7 not supported" << std::endl;
            goto standard_format;
        }
        
        uint32_t F7_xmax, F7_ymax = 0;
        uint32_t F7_bmin, F7_bmax = 0;
        
        
        dc1394_format7_get_max_image_size(camera, video_mode, &F7_xmax, &F7_ymax);
        dc1394_format7_get_packet_parameters(camera, video_mode, &F7_bmin, &F7_bmax);
        
        if (config.frame_xoff==SETTING_MAX) config.frame_xoff = F7_xmax;
        if (config.frame_yoff==SETTING_MAX) config.frame_yoff = F7_ymax;
        
        uint32_t F7_x = config.frame_xoff;
        uint32_t F7_y = config.frame_yoff;
        
        uint32_t F7_w = config.frame_width;
        if (config.frame_width==SETTING_MAX) F7_w = F7_xmax;
        if (F7_w>(F7_xmax-F7_x)) F7_w=F7_xmax-F7_x;
        if (F7_w%2!=0) F7_w--; // only even numbers
        
        uint32_t F7_h = config.frame_height;
        if (config.frame_height==SETTING_MAX) F7_h = F7_ymax;
        if (F7_h>(F7_ymax-F7_y)) F7_h=F7_ymax-F7_y;
        if (F7_h%2!=0) F7_h--; // only even numbers
        
        uint32_t F7_b = F7_bmax;
        //uint32_t F7_b = F7_bmax/5*4;
        
        dc1394_get_color_coding_from_video_mode(camera,video_mode, &coding);
        if ((coding!=preferred_coding) &&  (coding!=YUV411_coding) &&  (coding!=YUV422_coding)) printf("problems setting color coding\n");
        
        if ( dc1394_video_set_mode(camera, video_mode) !=DC1394_SUCCESS) {
            std::cerr << "error getting FORMAT7" << std::endl;
            goto standard_format;
        }
        
        if ( dc1394_format7_set_roi(camera, video_mode, coding, F7_b,F7_x, F7_y, F7_w, F7_h) !=DC1394_SUCCESS) {
            std::cerr << "error setting up FORMAT7" << std::endl;
            goto standard_format;
        }
        
    } else { standard_format:
        // get video modes
        if (dc1394_video_get_supported_modes(camera,&video_modes)!=DC1394_SUCCESS) {
            fprintf(stderr,"can't get video modes\n");
            //if (camera!=NULL) dc1394_camera_free(camera);
            return false;
        }
        
        if ((config.cam_width!=SETTING_MAX) || (config.cam_height!=SETTING_MAX)) {
            for (int i=video_modes.num-1;i>=0;i--) {
                if (!dc1394_is_video_mode_scalable(video_modes.modes[i])) {
                    dc1394_get_color_coding_from_video_mode(camera,video_modes.modes[i], &coding);
                    if ((config.cam_width==width_table[video_modes.modes[i]-DC1394_VIDEO_MODE_MIN]) && (config.cam_height==height_table[video_modes.modes[i]-DC1394_VIDEO_MODE_MIN])) {
                        if ((coding==preferred_coding) || (coding==YUV411_coding) || (coding==YUV422_coding)) {
                            video_mode=video_modes.modes[i];
                            break;
                        }
                    }
                }
            }
        }
        
        // select highest res mode
        if (video_mode == DC1394_VIDEO_MODE_FORMAT7_0) {
            unsigned int w=0;
            unsigned int h=0;
            for (int i=video_modes.num-1;i>=0;i--) {
                if (!dc1394_is_video_mode_scalable(video_modes.modes[i])) {
                    dc1394_get_color_coding_from_video_mode(camera,video_modes.modes[i], &coding);
                    if ((coding==preferred_coding) || (coding==YUV411_coding) || (coding==YUV422_coding)) {
                        unsigned int maxw = w;
                        dc1394_get_image_size_from_video_mode(camera, video_modes.modes[i], &w, &h);
                        if ((video_mode != DC1394_VIDEO_MODE_FORMAT7_0) && (w<maxw)) break;
                        video_mode=video_modes.modes[i];
                        if (coding!=YUV422_coding) break;
                    }
                }
            }
        }
        
        
        if (video_mode == DC1394_VIDEO_MODE_FORMAT7_0) {
            fprintf(stderr,"can't get suitable  video mode\n");
            //if (camera!=NULL)  dc1394_camera_free(camera);
            return false;
        }
        
        // get highest framerate
        if (dc1394_video_get_supported_framerates(camera,video_mode,&framerates)!=DC1394_SUCCESS) {
            fprintf(stderr,"can't get framrates\n");
            //if (camera!=NULL)  dc1394_camera_free(camera);
            return false;
        }
        
        framerate=framerates.framerates[framerates.num-1];
        for (unsigned int i=0;i<framerates.num;i++) {
            if (config.cam_fps == framerate_table[framerates.framerates[i]-DC1394_FRAMERATE_MIN]) {
                framerate=framerates.framerates[i];
                break;
            }
        }
        
        dc1394_video_set_framerate(camera, framerate);
        dc1394_video_get_framerate(camera, &framerate);
        this->fps = framerate_table[framerate-DC1394_FRAMERATE_MIN];
        
        dc1394_video_set_mode(camera, video_mode);
        dc1394_video_set_framerate(camera, framerate);
    } //standard format
    
    
    if (dc1394_capture_setup(camera,4, DC1394_CAPTURE_FLAGS_DEFAULT)!=DC1394_SUCCESS) {
        fprintf( stderr,"unable to setup camera\n");
        //if (camera!=NULL) dc1394_camera_free(camera);
        return false;
    }
    
    unsigned int w, h;
    dc1394_get_image_size_from_video_mode(camera, video_mode, &w, &h);
    
    cam_width  = frame_width  = w;
    cam_height = frame_height = h;
    
    cam_buffer = new unsigned char[cam_width*cam_height*bytes];
    
   
    */
    /*
     what might we want to do here?
     we need to figure out the width and height, in order to figure out the buffer size
     */
    //unsigned int w, h;
    
    //could it be that the problem is that we simply don't have a camera handle yet?
    xiGetParamInt(handle, XI_PRM_WIDTH XI_PRM_INFO_MAX, &maxcx);
    xiGetParamInt(handle, XI_PRM_HEIGHT XI_PRM_INFO_MAX, &maxcy);
    
    
    
    cam_width  = frame_width  = maxcx;
    cam_height = frame_height = maxcy;
    cam_buffer = new unsigned char[cam_width*cam_height*bytes];
    //set default values
    
    //float fps=0;
    xiGetParamFloat(handle, XI_PRM_FRAMERATE, &fps);//use cameraEngine's fps instead of scoping it in here
    
    
    float mingain, maxgain;
    xiGetParamFloat(handle, XI_PRM_GAIN XI_PRM_INFO_MIN, &mingain);
    xiGetParamFloat(handle, XI_PRM_GAIN XI_PRM_INFO_MAX, &maxgain);
    
    xiSetParamInt(handle, XI_PRM_AUTO_WB, 1);
    xiSetParamInt(handle, XI_PRM_EXPOSURE, 10000);
    
    xiSetParamInt(handle, XI_PRM_IMAGE_DATA_FORMAT, XI_RAW8);
    xiSetParamInt(handle, XI_PRM_BUFFER_POLICY, XI_BP_SAFE);
    
    
    applyCameraSettings();
    
    return true;
     
     
}

unsigned char* USB3Camera::getFrame()
{
    //consider sing a pthread to do this ... so that we can easily dispose of the memory that is used by it 
    DWORD nIndex = 0;
    
    if(xiOpenDevice(nIndex, &handle) != XI_OK) {
        printf("Couldn't open camera!\n");
        //acquire = FALSE;
        //return TRUE;
    } else {
        printf("getFrame!\n");
    }
    
    if(xiGetImage(handle, 5000, &image) != XI_OK) {
        return NULL;
    } else {
        //we do have a handle
        
        XI_IMG src = image;
        //unsigned char *dest = cam_buffer;
        
        unsigned char *buffer = new unsigned char[cam_width*cam_height];
        
        /*for (int i=0;i<image.height; i++) {
            memcpy(cam_buffer, (image.bp+i*(image.width + image.padding_x)), image.width);
            
        }*/
        
        //printf("bytes=\n" + itoa(bytes));

        //what does cam_buffer = ?
        //printf("cam_buffer == " + cam_buffer);
        //cam_buffer = NULL;
        //let's try making a new buffer, for each frame here.
        if(!image.padding_x) {
            //gst_buffer_set_data(buffer, (guint8*)image.bp, image.width*image.height*(image.frm == XI_RAW8 ? 1 : 3));
            memcpy(buffer, (unsigned char*)image.bp, (image.width*(1)));
            printf("no padding!\n");
        } else {
            printf("yes padding!\n");
           for(int i = 0; i < image.height; i++) {
            //so ... this doesn't <not> work, but it doesn't really work either
            //
            memcpy(buffer, (unsigned char*)image.bp+i*(image.width*(1)+image.padding_x),
                   image.width*(1));
            
           }
        }
       
        return buffer;
    }
    
    
    
    /*
    dc1394video_frame_t *frame = NULL;
    dc1394_capture_dequeue(camera, DC1394_CAPTURE_POLICY_POLL, &frame);
    if (frame==NULL) {
        if (lost_frames>timeout) { // give up after 5 (at init) or 2 (at runtime) seconds
            running = false;
        } lost_frames++;
        usleep(1000);
        return NULL;
    }
    
    dc1394_capture_enqueue(camera, frame);
    lost_frames=0;
    timeout = 2000;
    
    if (colour) {
        if (coding==DC1394_COLOR_CODING_RGB8) return frame->image;
        else if (coding==DC1394_COLOR_CODING_YUV411) {
            
            int R,G,B;
            int Y1,Y2;
            int U,V;
            
            unsigned char *src = (unsigned char*)frame->image;
            unsigned char *dest = cam_buffer;
            for(int i=cam_height*cam_width/2;i>0;i--) {
                
                // U and V are +-0.5
                U    = *src++ - 128;
                Y1 = *src++;
                V    = *src++ - 128;
                Y2 = *src++;
                
                R = (int)(Y1 + 1.370705f * V);
                G = (int)(Y1 - 0.698001f * V - 0.337633f * U);
                B = (int)(Y1 + 1.732446f * U);
                
                if (R < 0) R=0; else if (R > 255) R=255;
                if (G < 0) G=0; else if (G > 255) G=255;
                if (B < 0) B=0; else if (B > 255) B=255;
                
                *dest++ = B;
                *dest++ = G;
                *dest++ = R;
                
                R = (int)(Y2 + 1.370705f * V);
                G = (int)(Y2 - 0.698001f * V - 0.337633f * U);
                B = (int)(Y2 + 1.732446f * U);
                
                if (R < 0) R=0; else if (R > 255) R=255;
                if (G < 0) G=0; else if (G > 255) G=255;
                if (B < 0) B=0; else if (B > 255) B=255;
                
                *dest++ = B;
                *dest++ = G;
                *dest++ = R;
            } return cam_buffer;
        } else { return NULL; }
    } else {
        
        if (coding==DC1394_COLOR_CODING_MONO8) return frame->image;
        else if (coding==DC1394_COLOR_CODING_YUV411) {
            unsigned char *src = (unsigned char*)frame->image;
            unsigned char *dest = cam_buffer;
            int size = cam_height*cam_width/2;
            for(int i=size;i>0;i--) {
                src++;
                *dest++ = *src++;
                *dest++ = *src++;
            } return cam_buffer;
        } else if (coding==DC1394_COLOR_CODING_YUV422) {
            unsigned char *src = (unsigned char*)frame->image;
            unsigned char *dest = cam_buffer;
            int size = cam_height*cam_width/2;
            for(int i=size;i>0;i--) {
                src++;
                *dest++ = *src++;
                src++;
                *dest++ = *src++;
            } return cam_buffer;
        } else { return NULL; }
    }
    */
    
    
    
}



bool USB3Camera::startCamera()
{
    DWORD tmp;
    xiGetNumberDevices(&tmp);//rescan available devices. It is needed to be called before any other function of API is called by application.
    DWORD nIndex = 0;
    HANDLE handle = INVALID_HANDLE_VALUE;
    
    if(xiOpenDevice(nIndex, &handle) != XI_OK) {
        printf("Couldn't setup camera!\n");
        //acquire = FALSE;
        //return TRUE;
    } else {
        printf("startCamera!\n");
    }
    
    if(xiStartAcquisition(handle) != XI_OK) {
        fprintf(stderr,"camera doesn't seem to want to turn on\n");
        return false;
    } else {
        return true;
    
    }
    /*
    if (dc1394_video_set_transmission(camera, DC1394_ON) !=DC1394_SUCCESS) {
        fprintf( stderr, "unable to start camera iso transmission\n");
        //if (camera!=NULL) dc1394_camera_free(camera);
    }
    
    dc1394switch_t status = DC1394_OFF;
    
    int i = 0;
    while( status == DC1394_OFF && i++ < 5 ) {
        usleep(50000);
        if (dc1394_video_get_transmission(camera, &status)!=DC1394_SUCCESS) {
            fprintf(stderr, "unable to get transmision status\n");
            return false;
        }
    }
    
    if( i == 5 ) {
        fprintf(stderr,"camera doesn't seem to want to turn on\n");
        return false;
    }
     */
    
    
    running = true;
    return true;
}

bool USB3Camera::stopCamera()
{
    /*
    if(!running) return false;
    
    if (dc1394_video_set_transmission(camera,DC1394_OFF)!=DC1394_SUCCESS) {
        //printf("couldn't stop the camera?\n");
        //return false;
    }
    
    if (dc1394_capture_stop(camera)!=DC1394_SUCCESS) {
        //printf("couldn't stop the camera?\n");
        //return false;
    }
    
    */
    
    DWORD nIndex = 0;
    HANDLE handle = INVALID_HANDLE_VALUE;
    if(xiOpenDevice(nIndex, &handle) != XI_OK) {
        printf("Couldn't stopCamera camera!\n");
        //acquire = FALSE;
        //return TRUE;
    } else {
        printf("stopCamera!\n");
    }
    
    xiStopAcquisition(handle);
    acquire = FALSE;
    running=false;
    return true;
}

bool USB3Camera::stillRunning() {
    return running;
}

bool USB3Camera::resetCamera()
{
    return (stopCamera() && startCamera());
}

bool USB3Camera::closeCamera()
{
    //stopCamera();
    xiCloseDevice(handle);
    handle = INVALID_HANDLE_VALUE;
    running=false;
    
    return true;
}

bool USB3Camera::setCameraSettingAuto(int mode, bool flag) {
    /*
    dc1394feature_t feature = (dc1394feature_t)-1;
    switch (mode) {
        case BRIGHTNESS: feature = DC1394_FEATURE_BRIGHTNESS; break;
        case GAIN: feature = DC1394_FEATURE_GAIN; break;
        case SHUTTER: feature = DC1394_FEATURE_SHUTTER; break;
        case EXPOSURE: feature = DC1394_FEATURE_EXPOSURE; break;
        case SHARPNESS: feature = DC1394_FEATURE_SHARPNESS; break;
        case FOCUS: feature = DC1394_FEATURE_FOCUS; break;
        case GAMMA: feature = DC1394_FEATURE_GAMMA; break;
    } if (feature<0) return 0;
    
    if (flag==true) dc1394_feature_set_power(camera, feature, DC1394_ON);
    else dc1394_feature_set_power(camera, feature, DC1394_OFF);
    
    if ( dc1394_feature_set_mode(camera, feature, DC1394_FEATURE_MODE_AUTO) != DC1394_SUCCESS) return false;
    */
    
    //char * feature;
    std::string feature = "";
    //int feature;
    //char[] feature;
    switch (mode) {
        //case BRIGHTNESS: feature = XI_PRM_EXPOSURE; break;
        case GAIN: feature = XI_PRM_GAIN; break;
        case SHUTTER: feature = XI_PRM_SHUTTER_TYPE; break;
        case EXPOSURE: feature = XI_PRM_EXPOSURE; break;
        case SHARPNESS: feature = XI_PRM_SHARPNESS; break;
        //case FOCUS: feature = XI_PRM_EXPOSURE; break;
        case GAMMA: feature = XI_PRM_GAMMAY; break;
    } if (feature == "") return false;

    return true;
}

bool USB3Camera::getCameraSettingAuto(int mode) {
    /*
    dc1394feature_t feature = (dc1394feature_t)-1;
    switch (mode) {
        case BRIGHTNESS: feature = DC1394_FEATURE_BRIGHTNESS; break;
        case GAIN: feature = DC1394_FEATURE_GAIN; break;
        case SHUTTER: feature = DC1394_FEATURE_SHUTTER; break;
        case EXPOSURE: feature = DC1394_FEATURE_EXPOSURE; break;
        case SHARPNESS: feature = DC1394_FEATURE_SHARPNESS; break;
        case FOCUS: feature = DC1394_FEATURE_FOCUS; break;
        case GAMMA: feature = DC1394_FEATURE_GAMMA; break;
    } if (feature<0) return 0;
    */
    return false;
}


bool USB3Camera::setCameraSetting(int mode, int setting) {
    /*
    int current_setting = getCameraSetting(mode);
    if (setting==current_setting) return true;
    setCameraSettingAuto(mode,false);
    
    dc1394feature_t feature = (dc1394feature_t)-1;
    switch (mode) {
        case BRIGHTNESS: feature = DC1394_FEATURE_BRIGHTNESS; config.brightness = setting; break;
        case GAIN: feature = DC1394_FEATURE_GAIN; config.gain = setting; break;
        case SHUTTER: feature = DC1394_FEATURE_SHUTTER; config.shutter = setting; break;
        case EXPOSURE: feature = DC1394_FEATURE_EXPOSURE; config.exposure = setting; break;
        case SHARPNESS: feature = DC1394_FEATURE_SHARPNESS; config.sharpness = setting; break;
        case FOCUS: feature = DC1394_FEATURE_FOCUS; config.focus = setting; break;
        case GAMMA: feature = DC1394_FEATURE_GAMMA; config.gamma = setting; break;
    } if (feature<0) return 0;
    
    uint32_t value = (uint32_t)setting;
    dc1394_feature_set_power(camera, feature, DC1394_ON);
    dc1394_feature_set_mode(camera, feature, DC1394_FEATURE_MODE_MANUAL);
    if (dc1394_feature_set_value(camera, feature, value)!= DC1394_SUCCESS) return false;
    */
    return true;
}

int USB3Camera::getCameraSetting(int mode) {
    /*
    dc1394feature_t feature = (dc1394feature_t)-1;
    switch (mode) {
        case BRIGHTNESS: feature = DC1394_FEATURE_BRIGHTNESS; break;
        case GAIN: feature = DC1394_FEATURE_GAIN; break;
        case SHUTTER: feature = DC1394_FEATURE_SHUTTER; break;
        case EXPOSURE: feature = DC1394_FEATURE_EXPOSURE; break;
        case SHARPNESS: feature = DC1394_FEATURE_SHARPNESS; break;
        case FOCUS: feature = DC1394_FEATURE_FOCUS; break;
        case GAMMA: feature = DC1394_FEATURE_GAMMA; break;
    } if (feature<0) return 0;
    
    uint32_t value = 0;
    if (dc1394_feature_get_value(camera,  feature, &value)!= DC1394_SUCCESS) return 0;
     */
    
    uint32_t value = 0;
    
    return (int)value;
}

int USB3Camera::getMaxCameraSetting(int mode) {
    
    /*
    dc1394feature_t feature = (dc1394feature_t)-1;
    switch (mode) {
        case BRIGHTNESS: feature = DC1394_FEATURE_BRIGHTNESS; break;
        case GAIN: feature = DC1394_FEATURE_GAIN; break;
        case SHUTTER: feature = DC1394_FEATURE_SHUTTER; break;
        case EXPOSURE: feature = DC1394_FEATURE_EXPOSURE; break;
        case SHARPNESS: feature = DC1394_FEATURE_SHARPNESS; break;
        case FOCUS: feature = DC1394_FEATURE_FOCUS; break;
        case GAMMA: feature = DC1394_FEATURE_GAMMA; break;
    } if (feature<0) return 0;
    
    uint32_t min;
    uint32_t max;
    if (dc1394_feature_get_boundaries(camera, feature, &min, &max)!= DC1394_SUCCESS) return 0;
     */
    uint32_t min;
    uint32_t max;
    return (int)max;
}

int USB3Camera::getMinCameraSetting(int mode) {
    /*
    dc1394feature_t feature = (dc1394feature_t)-1;
    switch (mode) {
        case BRIGHTNESS: feature = DC1394_FEATURE_BRIGHTNESS; break;
        case GAIN: feature = DC1394_FEATURE_GAIN; break;
        case SHUTTER: feature = DC1394_FEATURE_SHUTTER; break;
        case EXPOSURE: feature = DC1394_FEATURE_EXPOSURE; break;
        case SHARPNESS: feature = DC1394_FEATURE_SHARPNESS; break;
        case FOCUS: feature = DC1394_FEATURE_FOCUS; break;
        case GAMMA: feature = DC1394_FEATURE_GAMMA; break;
    } if (feature<0) return 0;
    
    uint32_t min;
    uint32_t max;
    if (dc1394_feature_get_boundaries(camera, feature, &min, &max)!= DC1394_SUCCESS) return 0;
     */
    uint32_t min;
    uint32_t max;
    
    return (int)min;
}

int USB3Camera::getCameraSettingStep(int mode) {
    /*
    dc1394feature_t feature = (dc1394feature_t)-1;
    switch (mode) {
        case BRIGHTNESS: feature = DC1394_FEATURE_BRIGHTNESS; break;
        case GAIN: feature = DC1394_FEATURE_GAIN; break;
        case SHUTTER: feature = DC1394_FEATURE_SHUTTER; break;
        case EXPOSURE: feature = DC1394_FEATURE_EXPOSURE; break;
        case SHARPNESS: feature = DC1394_FEATURE_SHARPNESS; break;
        case FOCUS: feature = DC1394_FEATURE_FOCUS; break;
        case GAMMA: feature = DC1394_FEATURE_GAMMA; break;
    } if (feature<0) return 0;
    */
    return 1;
}

bool USB3Camera::setDefaultCameraSetting(int mode) {
    
    return true;
}

int USB3Camera::getDefaultCameraSetting(int mode) {
    
    return INT_MIN;
}
