#include "icmp_codes.h"


// Every field that is labeled unused should be zero when packing data in the packet
// Each field should be included in the checksum 


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
	return code < 14 ? destination_unreachable_codes[code] : NULL;
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
	return code < 4 ? redirect_message_codes_meanings[code] : NULL;
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
	"Time To Live Exceeded",
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




/// <summary>
/// This function decodes icmp messages based on the type and the code sent from the host
/// </summary>
/// <param name="type">icmp type</param>
/// <param name="code">icmp code</param>
/// <returns>const char pointer to the meaning of the code and the type of the icmp header</returns>
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
	case 3: //destination unreachable 
		retval = get_destination_unreachable(code);
		break;
	case 4: //source quench
		retval = source_quench_codes_meaning[0];
		break;
	case 5: //redirect
		retval = get_redirect_message(code);
		break;
	case 6: //alternate host address
		retval = alterante_address[0];
		break;
	case 7: //unassigned
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

