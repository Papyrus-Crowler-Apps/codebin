#include <lvmx.h>
#include <jsmn.h>

int JSON_STRING[] = "[{\"Date\": \"date1\", \"Username\": \"user1\"}, {\"Date\": \"date2\", \"Username\": \"user2\"}, {\"Date\": \"date3\", \"Date\": \"user3\"}]";


int objAccess(int* json, jsmntok_t* t, int objitr, int* key) {
	int i;
	int jsonitr = objitr + 1;
	int pos = -1;
	int size = t[objitr].size;
	for (i = 0; i < size; i++) {

		json[t[jsonitr].end] = 0;

		if (strcmp(key, &json[t[jsonitr].start])) {
			pos = jsonitr + 1;
			break;
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

int calcJsonSize(jsmntok_t* t, int itr) {
	int size = t[itr++].size;
	int sum = 0;
	int counter = 0;

	for (; counter < size; itr++) {
		int type = t[itr].type;
		if (type == JSMN_OBJECT) {
			size += t[itr].size - 1;
		}else if (type == JSMN_ARRAY) {
			size += t[itr].size - 1;
		} else {
			if (t[itr].size == 0) counter++;
		}
		sum++;
	}

	return sum+1;
}

int main() {

	setDVInt(DEVICEROOT_SLOT, "OVERCLOCK", 30);

	jsmn_parser p;
	jsmntok_t t[128];
	int textbuff[32];

	jsmn_init(&p);
	int r = jsmn_parse(&p, JSON_STRING, strlen(JSON_STRING), t, sizeof(t));


	if (t[0].type != JSMN_ARRAY) {
		debuglog("ARRAY expected\n");
		return -1;
	}

	int size = t[0].size;

	int i;
	int pos = 1;
	for (i = 0; i < size; i++) {

		int strid = objAccess(JSON_STRING, t, pos, "Date");
		getString(JSON_STRING, t, strid, textbuff);
		debuglog(textbuff);
		debuglog("\n");

		pos += calcJsonSize(t, pos);

	}
	return -1;

}
