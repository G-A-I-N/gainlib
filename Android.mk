LOCAL_PATH:= $(call my-dir)
 
include $(CLEAR_VARS)
 
LOCAL_MODULE := libgain

LOCAL_SRC_FILES :=  GainApp.cpp\
					Core.cpp\
					Base.cpp\
					Rect.cpp\
					Layer.cpp\
					EffectLayer.cpp\
					Menu.cpp\
					Button.cpp\
					PixelCanvas.cpp\
					Bitmap.cpp\
					ImageBitmap.cpp\
					FboBitmap.cpp\
					Text.cpp\
					TouchInterface.cpp \
					EventListener.cpp \
					Timer.cpp\
					Logger.cpp \
					PerfomanceCounter.cpp \
					PerfCounterItem.cpp \
					Settings.cpp \
					Slider.cpp
					
LOCAL_C_INCLUDES := \
                    gain

LOCAL_LDLIBS    := -llog -lGLESv2 -landroid -lEGL

include $(BUILD_SHARED_LIBRARY)
 
$(call import-add-path, gain ) 
 
