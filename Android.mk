LOCAL_PATH:= $(call my-dir)
 
include $(CLEAR_VARS)
 
LOCAL_MODULE := libgain

LOCAL_SRC_FILES :=  GainApp.cpp\
					Core.cpp\
					Base.cpp\
					Rect.cpp\
					ShaderTest.cpp\
					PixelCanvas.cpp\
					Bitmap.cpp\
					PngBitmap.cpp\
					Text.cpp\
					TouchInterface.cpp \
					Logger.cpp
					
LOCAL_C_INCLUDES := $(LOCAL_PATH)/freetype/include \
                    $(LOCAL_PATH)/freetype/include/freetype2 \
                    $(LOCAL_PATH)/libpng/include/libpng16 \
                    $(LOCAL_PATH)

LOCAL_LDLIBS    := -llog -lGLESv2 -landroid -lEGL -lz  

LOCAL_STATIC_LIBRARIES := freetype libpng

include $(BUILD_SHARED_LIBRARY)
 
$(call import-add-path, jni/gain ) 
$(call import-module , freetype )
$(call import-module , libpng )
 
