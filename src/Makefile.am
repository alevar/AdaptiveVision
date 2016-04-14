bin_PROGRAMS = server client
server_SOURCES = server.cpp	MessageOpCode.cpp Message.cpp MessageType.cpp MessageStack.cpp MessageQueue.cpp MessageException.cpp PID.cpp Histogram.cpp CameraDetect.cpp ObjectIdentification.cpp Compressor.cpp Decompressor.cpp Template.cpp MatchMSER.cpp MatchHSV.cpp Sample.cpp Canvas.cpp
client_SOURCES = client.cpp MessageOpCode.cpp Message.cpp MessageType.cpp MessageStack.cpp MessageQueue.cpp MessageException.cpp PID.cpp Histogram.cpp CameraDetect.cpp ObjectIdentification.cpp Compressor.cpp Decompressor.cpp Template.cpp MatchMSER.cpp MatchHSV.cpp Sample.cpp Canvas.cpp

server_LDADD = -L/usr/local/lib -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -L/usr/lib/x86_64-linux-gnu 
client_LDADD = -L/usr/local/lib -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -L/usr/lib/x86_64-linux-gnu 

server_LFLAGS = -static
client_LFLAGS = -static

server_LDFLAGS ='-I/usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-3.0-unofficial' -D_FILE_OFFSET_BITS=64 -I/usr/include/wx-3.0-unofficial -DWXUSINGDLL  -D__WXGTK__ -pthread
client_LDFLAGS ='-I/usr/lib/x86_64-linux-gnu/wx/include/gtk2-unicode-3.0-unofficial' -D_FILE_OFFSET_BITS=64 -I/usr/include/wx-3.0-unofficial -DWXUSINGDLL  -D__WXGTK__ -pthread