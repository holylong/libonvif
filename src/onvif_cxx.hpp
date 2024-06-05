#ifndef _ONVIF_CXX_HPP_
#define _ONVIF_CXX_HPP_

#include "onvif_interface.h"

#include <functional>
#include <string>
#include <vector>

#include <iostream>
#include <vector>
#include <string>
#include "onvif_interface.h"
#include "onvif_comm.h"
#include "onvif_dump.h"

class ONVIFDevice {
public:
    ONVIFDevice(){
        instance = this;
    };
    ~ONVIFDevice(){
        instance = nullptr;
    };

    // 初始化设备发现
    void detectDevices() {
        MYONVIF_DetectDevice(discoveryCallback);
    }

    // 获取设备的 URI 列表
    std::vector<std::string> getURIs() const {
        return uriAuthList;
    }

private:
    static void discoveryCallback(char* DeviceXAddr){
        int stmno = 0;
        int profile_cnt = 0;
        struct tagProfile* profiles = nullptr;
        struct tagCapabilities capa;

        char uri[MY_ONVIF_ADDRESS_SIZE] = {0};
        char uri_auth[MY_ONVIF_TOKEN_SIZE + 50] = {0};

        if (MYONVIF_GetCapabilities(DeviceXAddr, &capa) == 0) {
            profile_cnt = MYONVIF_GetProfiles(capa.MediaXAddr, &profiles);

            if (profile_cnt > stmno) {
                if (MYONVIF_GetStreamUri(capa.MediaXAddr, profiles[stmno].token, uri, sizeof(uri)) == 0) {
                    char chName[6] = "admin";
                    if (my_make_uri_withauth(uri, chName, chName, uri_auth, sizeof(uri_auth)) == 0) {
                        if (instance) {
                            instance->uriAuthList.push_back(uri_auth);
                        }
                    }
                }
            }

            if (profiles) {
                free(profiles);
            }
        }
    }

    std::vector<std::string> uriAuthList;
    static ONVIFDevice* instance; // 用于回调函数访问实例

    // 禁止拷贝和赋值
    ONVIFDevice(const ONVIFDevice&) = delete;
    ONVIFDevice& operator=(const ONVIFDevice&) = delete;
};

#endif
