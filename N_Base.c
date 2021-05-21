#include "N_Base.h"
#include <stdint.h>

static int baseN_get_divisor(char *datas, int dataslen ,int srcBase ,int dstBase ,char *out_buff ,int out_buffsz ,int *pRemainder)
{
	uint32_t value = 0;
	uint32_t divisor = 0 ,remainder = 0; 
	int index ,res = 0 ,idx = 0;

	for(index = 0; index < dataslen; index++){
		/*Convert value */
		value = (value * srcBase) + datas[index] - '0';
		divisor = value / dstBase;
		remainder = value % dstBase;
		if(divisor > 0){//Enough to divide 
			value = remainder;
		}

		if(!res && ((divisor > 0) || ((index + 1) == dataslen))){
			res = 1;
		}

		if(res){
			out_buff[idx] = divisor + '0';
			idx++;
			if(idx > out_buffsz){
				return (-1);
			}
		}
	}
	*pRemainder = remainder;

	return (idx);
}

/// <summary>
/// N-Base conversion 
/// </summary>
/// <param name="datas ,dataslen">Data input to be converted</param>
/// <param name="srcBase">Input data base number</param>
/// <param name="dstBase">Output data base</param>
/// <param name="out_buff ,out_buffsz">Output buffer </param>
/// <returns></returns>
int baseN_encode(char *datas, int dataslen ,int srcBase ,int dstBase ,char *out_buff ,int out_buffsz)
{
	const char strkey[] = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int cnt ,idx = 0 ,remainder;

	cnt = dataslen;

	do{
		cnt = baseN_get_divisor(datas ,cnt ,srcBase ,dstBase ,datas ,dataslen ,&remainder);
		if(cnt <= 0 || (cnt == 1 && remainder == 0 && datas[0] == '0')){/*End of data conver*/
			cnt = 0;
		}else{/*Encode*/
			out_buff[idx] = strkey[remainder];
			idx++;
			if(idx > out_buffsz){
				return (-1);
			}
		}
	}while(cnt > 0);

	/*reversal*/
	cnt = idx - 1;
	for (int n = 0; n < (idx / 2); n++)
	{
		remainder = out_buff[n];
		out_buff[n] = out_buff[cnt];
		out_buff[cnt] = remainder;
		cnt--;
	}

	out_buff[idx] = 0;

	return idx;
}