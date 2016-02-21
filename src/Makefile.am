bin_PROGRAMS = server client
server_SOURCES = server.cpp	MessageOpCode.cpp Message.cpp MessageType.cpp MessageStack.cpp MessageQueue.cpp MessageException.cpp PID.cpp Histogram.cpp CameraDetect.cpp ObjectIdentification.cpp MessageCompress.cpp
client_SOURCES = client.cpp MessageOpCode.cpp Message.cpp MessageType.cpp MessageStack.cpp MessageQueue.cpp MessageException.cpp PID.cpp Histogram.cpp CameraDetect.cpp ObjectIdentification.cpp MessageCompress.cpp

server_LDADD = -L/usr/local/lib -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core
client_LDADD = -L/usr/local/lib -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core

server_LFLAGS = -static
client_LFLAGS = -static