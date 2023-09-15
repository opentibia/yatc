CXXFLAGS=-DCLI_ONLY -DLOGIN_ONLY -DHAVE_GMP_H=1
CFLAGS=-DCLI_ONLY -DLOGIN_ONLY -DHAVE_GMP_H=1
OBJS=../net/connection.o ../net/networkmessage.o ../debugprint.o ../util.o notifications.o ../gamecontent/creature.o ../gamecontent/container.o ../gamecontent/globalvars.o ../gamecontent/inventory.o ../gamecontent/map.o creatureui.o thingui.o distanceui.o effectui.o ../net/protocolconfig.o ../net/rsa.o ../net/encryption.o ../net/protocollogin.o
all: yatc_cli dat2json_cli
yatc_cli: $(OBJS) yatc_cli.o objects.o
	g++ yatc_cli.o $(OBJS) objects.o -lSDL -lgmp -o yatc_cli
dat2json_cli: $(OBJS) dat2json_cli.o ../objects.o ../options.o ../confighandler.o
	g++ dat2json_cli.o ../objects.o ../options.o ../confighandler.o $(OBJS) -lSDL -lgmp -o dat2json_cli
clean:
	rm $(OBJS) yatc_cli yatc_cli.o dat2json_cli dat2json_cli.o ../objects.o objects.o ../options.o ../confighandler.o
