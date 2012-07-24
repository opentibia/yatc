#ifdef __cplusplus
extern "C" {
#endif
	
#ifndef MACUTIL_H
#define MACUTIL_H
#ifdef __APPLE__
	int macutil_NSRunAlertPanel(const char* title, const char* text, const char *btn1, const char *btn2, const char *btn3,...);
#endif
#endif
	
#ifdef __cplusplus
}
#endif