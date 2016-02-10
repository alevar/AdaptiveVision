bin_PROGRAMS = server client
server_SOURCES = server.cpp	MessageOpCode.cpp Message.cpp MessageType.cpp MessageError.cpp MessageStack.cpp MessageQueue.cpp MessageException.cpp PID.cpp ActionCode.h
client_SOURCES = client.cpp MessageOpCode.cpp Message.cpp MessageType.cpp MessageError.cpp MessageStack.cpp MessageQueue.cpp MessageException.cpp PID.cpp ActionCode.h
server_LDADD = -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal 
client_LDADD = -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal 
server_LFLAGS = -static
client_LFLAGS = -static