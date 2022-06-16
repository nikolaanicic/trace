#include "icmp_codes.h"


const char* destination_unreachable_codes[14] =
{
	"Destination network unreachable",
	"Destination host unreachable",
	"Destination protocol unreachable",
	"Destination port unrachable",
	"Fragmentation required, DF set",
	"Source route failed",
	"Destination network unkown",
	"Destination host unkown",
	"Source host isolated",
	"Network administratively prohibited",
	"Host administratively prohibited",
	"Network unreachable for Type of Service",
	"Host unreachable for Type of Service",
	"Administratively prhoibited"
};

const char* get_destination_unreachable(int code)
{
	if (code < 14)
		return destination_unreachable_codes[code];

	return NULL;
}



const char* source_quench_codes_meaning[1] =
{
	"Traffic congestion control"
};

const char* redirect_message_codes_meanings[4] =
{
	"Redirect datagram for the Network",
	"Redirect datagram for the Host",
	"Redirect datagram for the Type of Service & Network",
	"Redirect datagram for the Type of Service & Host",
};


const char* get_redirect_message(int code)
{
	if (code < 4)
		return redirect_message_codes_meanings[code];
	return NULL;
}

const char* alterante_address[1] =
{
	"Alternate Host Address (Deprecated)"
};


const char* types_codes_meanings[41] =
{
	"Echo reply",
	"Reserved",
	"Reserved",
	"",
	"",
	"",
	"",
	"",
	"Echo Request",
	"Router Advertisement",
	"Router Solicitation",
	"Time Exceeded",
	"Parameter Problem",
	"Timestamp Request",
	"Timestamp Reply",
	"Information Request",
	"Information Reply",
	"Address Mask Request",
	"Address Mask Reply",
	"Reserved for security",
	"Reserved for robustness experiment",
	"Reserved for robustness experiment",
	"Reserved for robustness experiment",
	"Reserved for robustness experiment",
	"Reserved for robustness experiment",
	"Reserved for robustness experiment",
	"Reserved for robustness experiment",
	"Reserved for robustness experiment",
	"Reserved for robustness experiment",
	"Reserved for robustness experiment",
	"Information Request",
	"Datagram conversion error",
	"Mobile Host Redirect",
	"Where-Are-You",
	"Here-I-Am",
	"Mobile Registration Request",
	"Mobile Registration Reply",
	"Domain Name Request",
	"SKIP Algorithm Discovery Protocol",
	"Photuris (Firefly) security protocol",
	"ICMP for experimental mobility protocols such as Seamboy"
};


const char* get_code_meaning(int type, int code)
{
	const char* retval = NULL;

	switch (type)
	{
	case 0: // Echo reply message
		retval = types_codes_meanings[0];
		break;
	case 1:
		retval = types_codes_meanings[1];
		break;
	case 2:
		retval = types_codes_meanings[2];
		break;
	case 3:
		retval = get_destination_unreachable(code);
		break;
	case 4:
		retval = source_quench_codes_meaning[0];
		break;
	case 5:
		retval = get_redirect_message(code);
		break;
	case 6:
		retval = alterante_address[0];
		break;
	case 7:
		retval = "Reserved";
		break;
	default:
		if (type < 41)
			retval = types_codes_meanings[type];
		else
			retval = NULL;
	}

	return retval;
}

