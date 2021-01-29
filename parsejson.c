
#include <lvmx.h>
#include <jsmn.h>

int JSON_STRING[] = "{\"user\": \"johndoe\", \"admin\": false, \"uid\": 1000,\"groups\": [\"users\", \"wheel\", \"audio\", \"video\"]}";


int getElem(int* json, jsmntok_t* t, int* key, int size) {
	int i;
	int jsonitr = 1;
	int pos = -1;
	for (i = 0; i < size; i++) {

		json[t[jsonitr].end] = 0;

		if (strcmp(key, &json[t[jsonitr].start])) {
			debuglog("found!\n");
			pos = jsonitr + 1;
			//break;
		}

		jsonitr += t[jsonitr].size + 1;
	}

	return pos;
}

int getString(int* json, jsmntok_t* t, int id, int* dest) {
	if (t[id].type != JSMN_STRING) {
		debuglog("String expected\n");
		return -1;
	}

	json[t[id].end] = 0;
	strcpy(dest, &json[t[id].start]);

	return 0;
}

int main() {

	setDVInt(DEVICEROOT_SLOT, "OVERCLOCK", 30);

	debuglog("hello\n");
	debuglog(JSON_STRING);
	debuglog("\n");
	jsmn_parser p;
	jsmntok_t t[32]; /* We expect no more than 128 tokens */

	jsmn_init(&p);
	int r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t));

	int textbuff[32];

	if (t[0].type != JSMN_OBJECT) {
		debuglog("Object expected\n");
		return -1;
	}

	int size = t[0].size;

	int tokItr = getElem(JSON_STRING, t, "user", size);
	getString(JSON_STRING, t, tokItr, textbuff);
	debuglog(textbuff);

	return -1;

}
