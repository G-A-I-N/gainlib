LOCAL_PATH:= $(call my-dir)
 
include $(CLEAR_VARS)
 
LOCAL_MODULE := libgain

LOCAL_SRC_FILES :=  GainApp.cpp\
					Core.cpp\
					Base.cpp\
					Layer.cpp\
					Menu.cpp\
					Rect.cpp\
					ShaderTest.cpp\
					PixelCanvas.cpp\
					Bitmap.cpp\
					PngBitmap.cpp\
					Text.cpp\
					TouchInterface.cpp \
					Logger.cpp \
					PerfomanceCounter.cpp \
					PerfCounterItem.cpp
					
LOCAL_C_INCLUDES := gain/freetype/include \
                    gain/freetype/include/freetype2 \
                    gain/libpng/include/libpng16 \
                    gain

LOCAL_LDLIBS    := -llog -lGLESv2 -landroid -lEGL -lz  

LOCAL_STATIC_LIBRARIES := freetype libpng

include $(BUILD_SHARED_LIBRARY)
 
$(call import-add-path, gain ) 
$(call import-module , freetype )
$(call import-module , libpng )
 
