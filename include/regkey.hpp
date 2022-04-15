
#ifndef AC110F21_6869_4FC1_9965_AE16E853880F
#define AC110F21_6869_4FC1_9965_AE16E853880F

#include <windows.h>

#include "mt_core.hpp"

template <typename keyType, int regType>
class regkey
{
protected:
	HKEY HK, hkey;
	const char* target_key;
	const char* target_value;

	keyType value;
	keyType enabledValue = 1;
	keyType disabledValue = 0;

	long lastError = ERROR_SUCCESS;

	void open(REGSAM access)
	{
		RegCreateKeyExA(HK,
			target_key,
			0, NULL, 0,
			access, NULL,
			&hkey, NULL);
	}
	void close()
	{
		RegCloseKey(hkey);
	}

public:
	regkey(const HKEY& HK, const char* target_key, const char* target_value) : HK(HK), target_key(target_key), target_value(target_value)
	{}
	regkey(const HKEY& HK, const char* target_key, const char* target_value, keyType enabledValue, keyType disabledValue) : HK(HK), target_key(target_key), target_value(target_value), enabledValue(enabledValue), disabledValue(disabledValue)
	{}

	keyType queryValue(unsigned long size = 1024)
	{
		open(KEY_READ);
		lastError = RegQueryValueEx(hkey,
			target_value,
			NULL, NULL,
			(LPBYTE)&value,
			&size);
		if (lastError != ERROR_SUCCESS)
		{
			LPSTR messageBuffer = nullptr;
			int size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				0, lastError, 0, (LPSTR)&messageBuffer, 256, 0);
			std::string message(messageBuffer, size - 1);
			LocalFree(messageBuffer);
			Warn(message << std::endl
				<< "\t" << target_key << ":" << target_value);
		}
		close();
		return value;
	}

	void setValue(keyType newValue)
	{
		open(KEY_WRITE);
		lastError = RegSetValueEx(hkey,
			target_value,
			0, regType,
			(BYTE*)&newValue,
			sizeof(newValue));
		if (lastError != ERROR_SUCCESS)
		{
			LPSTR messageBuffer = nullptr;
			int size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
				0, lastError, 0, (LPSTR)&messageBuffer, 256, 0);
			std::string message(messageBuffer, size - 1);
			LocalFree(messageBuffer);
			Warn(message << std::endl
				<< "\t" << target_key << ":" << target_value);
		}
		else
		{
			this->value = newValue;
		}
		close();
	}

	void enable()
	{
		setValue(enabledValue);
	}
	void disable()
	{
		setValue(disabledValue);
	}

	keyType getValue() const noexcept
	{
		return value;
	}
	keyType getEnabledValue() const noexcept
	{
		return enabledValue;
	}
	keyType getDisabledValue() const noexcept
	{
		return disabledValue;
	}

	long getLastError() const noexcept
	{
		return lastError;
	}

	const char* getTargetValue()const noexcept
	{
		return target_value;
	}

	const char* getTargetKey()const noexcept
	{
		return target_key;
	}
};

#endif /* AC110F21_6869_4FC1_9965_AE16E853880F */
