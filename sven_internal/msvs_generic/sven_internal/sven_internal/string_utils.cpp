#include "StdAfx.h"
#include "string_utils.hpp"

size_t Q_strnlen(const char* _String, size_t _NumberOfElements) {
    size_t cResult = 0;

    while (_String && *_String && _NumberOfElements) {
        cResult++;
        _NumberOfElements--;
        _String++;
    }

	return cResult;
}

size_t Q_strlen(const char* _String) {
    return Q_strnlen(_String, (size_t)-1);
}

int Q_strncpy_s(char* _Destination, size_t _NumberOfElements, const char* _Source, size_t _Count) {
	if (!_Destination || !_Source) return 1;
	if (_NumberOfElements == 0 || _Count == 0) return 1;

	const char* pszMaxOverlap = NULL;

	if (_Destination < _Source) {
		pszMaxOverlap = _Source;
        while (_NumberOfElements > 0) {
            if (_Destination == pszMaxOverlap) {
                return 1;
            }

            if (_Count == 0) {
                while (_NumberOfElements) { *_Destination = '\0'; _NumberOfElements--; _Destination++; }
                return 0;
            }

            *_Destination = *_Source;
            if (*_Destination == '\0') {
                while (_NumberOfElements) { *_Destination = '\0'; _NumberOfElements--; _Destination++; }
                return 0;
            }

            _NumberOfElements--;
            _Count--;
            _Destination++;
            _Source++;
        }
    } else {
        pszMaxOverlap = _Destination;

		while (_NumberOfElements > 0) {
			if (_Source == pszMaxOverlap) {
				return 1;
			}

			if (_Count == 0) {
				while (_NumberOfElements) { *_Destination = '\0'; _NumberOfElements--; _Destination++; }
				return 0;
			}

			*_Destination = *_Source;
			if (*_Destination == '\0') {
				while (_NumberOfElements) { *_Destination = '\0'; _NumberOfElements--; _Destination++; }
				return 0;
			}

            _NumberOfElements--;
			_Count--;
            _Destination++;
            _Source++;
		}
    }

    return 3;
}

int Q_strncat_s(char* _Destination, size_t _NumberOfElements, const char* _Source, size_t _Count) {
	if (!_Destination || !_Source) return 1;
	if (_NumberOfElements == 0 || _Count == 0) return 1;

	const char* pszMaxOverlap = NULL;
	if (_Destination < _Source) {
		pszMaxOverlap = _Source;
        while (*_Destination != '\0') {

            if (_Destination == pszMaxOverlap) {
                return 1;
            }

            _Destination++;
            _NumberOfElements--;
            if (_NumberOfElements == 0) {
                return 1;
            }
        }

        while (_NumberOfElements > 0) {
            if (_Destination == pszMaxOverlap) {
                return 1;
            }

            if (_Count == 0) {
                while (_NumberOfElements) { *_Destination = '\0'; _NumberOfElements--; _Destination++; }
                return 0;
            }

            *_Destination = *_Source;
            if (*_Destination == '\0') {
                while (_NumberOfElements) { *_Destination = '\0'; _NumberOfElements--; _Destination++; }
                return 0;
            }

            _NumberOfElements--;
            _Count--;
            _Destination++;
            _Source++;
        }
	} else {
        pszMaxOverlap = _Destination;

        while (*_Destination != '\0') {
            _Destination++;
            _NumberOfElements--;
            if (_NumberOfElements == 0) {
                return 1;
            }
        }

        while (_NumberOfElements > 0) {
            if (_Source == pszMaxOverlap) {
                return 1;
            }

            if (_Count == 0) {
                while (_NumberOfElements) { *_Destination = '\0'; _NumberOfElements--; _Destination++; }
                return 0;
            }

            *_Destination = *_Source;
            if (*_Destination == '\0') {
                while (_NumberOfElements) { *_Destination = '\0'; _NumberOfElements--; _Destination++; }
                return 0;
            }

            _NumberOfElements--;
            _Count--;
            _Destination++;
            _Source++;
        }
    }

    return 4;
}

char Q_tolower(char _Character) {
	if (_Character >= 'A' && _Character <= 'Z') {
        _Character += ('a' - 'A');
	}

	return _Character;
}

char Q_toupper(char _Character) {
	if (_Character >= 'a' && _Character <= 'z') {
		_Character -= ('a' - 'A');
	}

	return _Character;
}

int Q_strncmp_s(const char* _String1, size_t _NumberOfElements, const char* _String2, size_t _Count) {
    if (!_String1 || !_String2 || !_NumberOfElements || !_Count) return 1;
    while (*_String1 && *_String2 && _NumberOfElements && _Count) {
        if (*_String1 != *_String2)
            break;

        _String1++;
        _String2++;
        _Count--;
        _NumberOfElements--;
    }

    return (*_String1 - *_String2);
}

int Q_strnicmp_s(const char* _String1, size_t _NumberOfElements, const char* _String2, size_t _Count) {
	if (!_String1 || !_String2 || !_NumberOfElements || !_Count) return 1;
	while (*_String1 && *_String2 && _NumberOfElements && _Count) {
		if (Q_tolower(*_String1) != Q_tolower(*_String2))
			break;

		_String1++;
		_String2++;
		_Count--;
		_NumberOfElements--;
	}

	return (Q_tolower(*_String1) - Q_tolower(*_String2));
}