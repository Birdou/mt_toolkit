
#ifndef D5E74BA6_FC6A_4367_B729_CBDAD85C588D
#define D5E74BA6_FC6A_4367_B729_CBDAD85C588D

#include <windows.h>

class sregkey
{
protected:
	HKEY HK, hkey;
	const char *target_key;
	const char *target_value;
	unsigned long regType;

	std::string value;

	long lastError = ERROR_SUCCESS;

	void open()
	{
		RegCreateKeyExA(HK,
						target_key,
						0, NULL, 0,
						KEY_WRITE | KEY_QUERY_VALUE, NULL,
						&hkey, NULL);
	}
	void close()
	{
		RegCloseKey(hkey);
	}

public:
	sregkey(const HKEY &HK, const char *target_key, const char *target_value, unsigned long regType, size_t buffsize = 512) : HK(HK), target_key(target_key), target_value(target_value), regType(regType)
	{
		value = new char[buffsize];
	}
	~sregkey()
	{
	}

	std::string queryValue(unsigned long size = 1024)
	{
		open();
		char *data = new char[size];
		lastError = RegQueryValueEx(hkey,
									target_value,
									NULL, NULL,
									(LPBYTE)data,
									&size);
		close();

		value = data;
		delete[] data;

		return value;
	}

	std::string getValue() const
	{
		return value;
	}

	void setValue(const std::string &newValue)
	{
		open();
		lastError = RegSetValueEx(hkey,
								  target_value,
								  0, regType,
								  (const BYTE *)newValue.c_str(),
								  newValue.length());
		if (lastError == ERROR_SUCCESS)
		{
			this->value = newValue;
		}
		close();
	}

	long getLastError() const
	{
		return lastError;
	}

	const char *getTargetValue()
	{
		return target_value;
	}

	const char *getTargetKey()
	{
		return target_key;
	}
};

#endif /* D5E74BA6_FC6A_4367_B729_CBDAD85C588D */
