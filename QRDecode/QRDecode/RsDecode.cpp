// RsDecode.cpp: implementation of the RsDecode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "QRDecodeDlg.h"
#include "RsDecode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
数组长度
sg0===npar
sg1===npar
sigma===npar/2+2
omega===npar/2+1
syn===npar
*/

RsDecode::RsDecode()
{
	galois = Galois();
}

RsDecode::~RsDecode()
{

}

RsDecode::RsDecode(int npar)
{
	galois = Galois();
	this->npar = npar;
}

/**
 * Modified Berlekamp-Massey
 *
 * @param sigma int[]
 *		σ(z)容量数组、需要最大npar/2 + 2个
 * 		σ0,σ1,σ2, ... σ<jisu>
 * @param omega int[]
 *		ω(z)容量数组、需要最大npar/2 + 1个
 *		ω0,ω1,ω2, ... ω<jisu-1>
 * @param syn int[]
 *		数组
 * 		s0,s1,s2, ... s<npar-1>
 * @return int
 * 		>= 0: σ的次数
 * 		< 0: 错误
 */
int RsDecode::calcSigmaMBM(int * sigma, int * omega, int * syn)
{
	int * sg0 = new int[npar];
	int * sg1 = new int[npar];
	int i;
	for(i=0;i<npar;i++)
	{
		sg0[i]=0;sg1[i]=0;
	}
	sg0[1] = 1;
	sg1[0] = 1;
	int jisu0 = 1;
	int jisu1 = 0;
	int m = -1;

	for(int n = 0; n < npar; n++) {
		// 计算判别式
		int d = syn[n];
		for(int i = 1; i <= jisu1; i++) {
			d ^= galois.mul(sg1[i], syn[n - i]);
		}
		if(d != 0) {
			int logd = galois.toLog(d);
			int * wk = new int[npar];
			for(int i=0;i<npar;i++)
				wk[i]=0;
			for(int i = 0; i <= n; i++) {
				wk[i] = sg1[i] ^ galois.mulExp(sg0[i], logd);
			}
			int js = n - m;
			if(js > jisu1) {
				m = n - jisu1;
				jisu1 = js;
				if(jisu1 > npar / 2) {
					return -1;				// σ的次数超过npar/2时，产生错误
				}
				for(int i = 0; i <= jisu0; i++) {
					sg0[i] = galois.divExp(sg1[i], logd);
				}
				jisu0 = jisu1;
			}
			sg1 = wk;
		}

		int cnt=npar-1<jisu0 ? npar-1 : jisu0;
		for(int i=cnt-1;i>=0;i--)
			sg0[i+1]=sg0[i];

		sg0[0] = 0;
		jisu0++;
	}
	galois.mulPoly(omega, sg1, syn, npar/2+1, npar, npar);

	int cnt=npar<npar/2+2 ? npar : npar/2+2;
	for(i=0;i<cnt;i++)
		sigma[i]=sg1[i];

	return jisu1;
}

/**
 * 求出错误位所在位置
 *		求出σ(z) = 0的解
 *		但是、要仅仅在数据长度范围内搜寻
 *		如果得出jisu个解、则说明出错
 * @param pos int[]
 * 		容纳错误位置的数组、jisu个
 * @param n int
 * 		数据长度
 * @param jisu int
 * 		σ的次书
 * @param sigma int[]
 * 		σ0,σ1,σ2, ... σ<jisu>
 * @return int
 *		0: 正常结束
 *		< 0: 出现错误
 */
int RsDecode::chienSearch(int * pos, int n, int jisu, int * sigma)
{
	/*
	 * σ(z) = (1-α^i*z)(1-α^j*z)(1-α^k*z)
	 *       = 1 + σ1z + σ2z^2 +...
	 * σ1 = α^i + α^j + α^k
	 * 通过上述性质进行最适化
	 * 从last = σ1开始、通过依次引入得到的解、最后的解即为last
	 */
	int last = sigma[1];

	if(jisu == 1) {
		// 如果次数为1，last即为次值
		if(galois.toLog(last) >= n) {
			return RS_CORRECT_ERROR;	// 超出范围，出错
		}
		pos[0] = last;
		return 0;
	}

	int posIdx = jisu - 1;		// 用于存储索引的错误位置
	for(int i = 0; i < n; i++) {
		/*
		 * σ(z)的计算
		 * w 用1(0乘的项)初始化后、剩余的项目用<1..jisu>进行加算
		 * z = 1/α^i = α^Iとすると
		 * σ(z) = 1 + σ1α^I + σ2(α^I)^2 + σ3(α^I)^3 + ... + σ<jisu>/(α^I)^<jisu>
		 *       = 1 + σ1α^I + σ2α^(I*2) + σ3α^(I*3) + ... + σ<jisu>α^(I*<jisu>)
		 */
		int z = 255 - i;					// z = 1/α^i
		int wk = 1;
		for(int j = 1; j <= jisu; j++) {
			wk ^= galois.mulExp(sigma[j], (z * j) % 255);
		}
		if(wk == 0) {
			int pv = galois.toExp(i);		// σ(z) = 0的解
			last ^=  pv;					// 通过last引入解
			pos[posIdx--] = pv;
			if(posIdx == 0) {
				// 如果剩余为1、last即为此解
				if(galois.toLog(last) >= n) {
					return RS_CORRECT_ERROR;	// 最终解在范围之外，错误
				}
				pos[0] = last;
				return 0;
			}
		}
	}
	// 索范围内的数据长度，该解决方案不能得出结果
	return RS_CORRECT_ERROR;
}

/**
 * Forney错误纠正法
 *		σ(z) = (1-α^i*z)(1-α^j*z)(1-α^k*z)
 *		σ'(z) = α^i * (1-α^j*z)(1-α^k*z)...
 *			   + α^j * (1-α^i*z)(1-α^k*z)...
 *			   + α^k * (1-α^i*z)(1-α^j*z)...
 *		ω(z) = (E^i/(1-α^i*z) + E^j/(1-α^j*z) + ...) * σ(z)
 *			  = E^i*(1-α^j*z)(1-α^k*z)...
 *			  + E^j*(1-α^i*z)(1-α^k*z)...
 *			  + E^k*(1-α^i*z)(1-α^j*z)...
 *		∴ E^i = α^i * ω(z) / σ'(z)
 * @param data int[]
 *		数据输入数组
 * @param length int
 *		数据输入长度
 * @param jisu int
 *		σの次数
 * @param pos int[]
 *		错误位置数组
 * @param sigma int[]
 *		σ0,σ1,σ2, ... σ<jisu>
 * @param omega int[]
 *		ω0,ω1,ω2, ... ω<jisu-1>
 */
void RsDecode::doForney(BYTE * data, int length, int jisu, int * pos, int * sigma, int * omega)
{
	int i,j;
	for(i = 0; i < jisu; i++)
	{
		int ps = pos[i];
		int zlog = 255 - galois.toLog(ps);					// z标量

		// 计算ω(z)
		int ov = omega[0];
		for(j = 1; j < jisu; j++)
		{
			ov ^= galois.mulExp(omega[j], (zlog * j) % 255);		// ov += ωi * z^j
		}

		// 计算σ'(z)的值(σ(z)的微分)
		int dv = sigma[1];
		for(j = 2; j < jisu; j += 2)
		{
			dv ^= galois.mulExp(sigma[j + 1], (zlog * j) % 255);	// dv += σ<j+1> * z^j
		}

		/*
		 * 错误纠正 E^i = α^i * ω(z) / σ'(z)
		 * 在特定范围内的搜索的时间保证在错误的位置
		 * 它不会在这里检查
		 */
		data[galois.toPos(length, ps)] ^= galois.mul(ps, galois.div(ov, dv));
	}
}

/**
 * RS码的解码
 *
 * @param data int[]
 *		数据输入数组
 * @param length int
 * 		数据长度，包括校验
 * @param noCorrect boolean
 * 		只检查，不纠错
 * @return int
 * 		0: 无错误
 * 		> 0: 纠正了个别错误的返回值
 * 		< 0: 不能纠正
 */
int RsDecode::decode(BYTE * data, int length, boolean noCorrect)
{
	if(length < npar || length > 255) {
		return RS_PERM_ERROR;
	}
	// 综合计算
	int * syn = new int[npar];
	for(int i=0;i<npar;i++)syn[i]=0;

	if(galois.calcSyndrome(data, length, syn, npar)) {
		return 0;		// 无措
	}
	//  求出σ和ω
	int * sigma = new int[npar/2+2];
	for(int i=0;i<npar/2+2;i++)sigma[i]=0;
	int * omega = new int[npar/2+1];
	for(int i=0;i<npar/2+1;i++)omega[i]=0;
	int jisu = calcSigmaMBM(sigma, omega, syn);
	if(jisu <= 0) {
		return RS_CORRECT_ERROR;
	}
	// 通过探索求出错误的位置
	int * pos = new int[jisu];
	for(int i=0;i<jisu;i++)pos[i]=0;

	int r = chienSearch(pos, length, jisu, sigma);
	if(r < 0) {
		return r;
	}
	if(!noCorrect) {
		// 纠错
		doForney(data, length, jisu, pos, sigma, omega);
	}
	return jisu;
}

int RsDecode::decode(BYTE * data, int length)
{
	return decode(data, length, false);
}
