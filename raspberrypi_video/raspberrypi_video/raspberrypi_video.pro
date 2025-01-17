TEMPLATE = app
QT += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = raspberrypi_video

RPI_LIBS = ../raspberrypi_libs
LEPTONSDK = leptonSDKEmb32PUB

PRE_TARGETDEPS += sdk
QMAKE_EXTRA_TARGETS += sdk sdkclean
sdk.commands = make -C $${RPI_LIBS}/$${LEPTONSDK}
sdkclean.commands = make -C $${RPI_LIBS}/$${LEPTONSDK} clean

DEPENDPATH += .
INCLUDEPATH += . $${RPI_LIBS}

DESTDIR=.
OBJECTS_DIR=gen_objs
MOC_DIR=gen_mocs

HEADERS += *.h

SOURCES += *.cpp

unix:LIBS += -L$${RPI_LIBS}/$${LEPTONSDK}/Debug -lLEPTON_SDK

unix:QMAKE_CLEAN += -r $(OBJECTS_DIR) $${MOC_DIR}

# CURL과 JsonCpp 라이브러리 추가
LIBS += -lcurl -ljsoncpp

# 클라우드 함수 추가
def receive_temperature(request):
    from datetime import datetime
    from google.cloud import datastore
    
    # 요청에서 데이터 추출
    request_json = request.get_json()
    
    # Datastore 클라이언트 생성
    client = datastore.Client()
    
    # 엔티티 생성
    key = client.key('Temperature')
    entity = datastore.Entity(key=key)
    entity.update({
        'temperature': request_json['temperature'],
        'timestamp': datetime.fromtimestamp(request_json['timestamp']),
        'device_id': request_json['device_id']
    })
    
    # Datastore에 저장
    client.put(entity)
    
    return 'OK', 200
        



