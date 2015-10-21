/**
 * @file ssm2603.c
 * @author: takemasa
 * @brief Analog Devices SSM2603初期化プログラム
 * @details
 * SSM2603の初期化を行う。I2Cを使って初期化を行う。
 */

#include "i2s_subsystem.h"
#include "i2c_subsystem.h"
/**
 * @brief コーデック初期化データ列の長さ
 * @details
 * TLV320AIC23に与える初期化データの長さである。長さはレジスタアドレスとデータの組が
 * 何組であるかを示す。
 */
#define     CODECINITDATALEN        11      /* TLV320AIC23B初期化データ長 */

/*
 * CODECのI2Cポート。
 * SSM2603が接続されているI2Cペリフェラルのポート番号。デフォルトは0である。他のポートを使いたい時には
 * sys_config.hで宣言すると良い。なお、I2Cコントローラ・ペリフェラルがひとつしかない場合には0を指定
 * する。
 */
#ifndef     UZUME_SSM2603_I2C_PORT
#define     UZUME_SSM2603_I2C_PORT   0
#endif

/*
 * TLV320AIC23 / SSM2603のI2Cデバイスアドレス(7bit右づめ)
 */
#ifndef     UZUME_SSM2603_I2C_ADRS
#define     UZUME_SSM2603_I2C_ADRS   0x1A
#endif

/**
 * @brief CODEC初期化データ列
 * @details
 * TLV320AIC23Bを初期化するためのデータ列である。
 */
static unsigned char tlv320aic23init[CODECINITDATALEN][2] =
{
                { 30 | 0x00, 0x00 },     /* RESET. */
                { 0 | 0x01, 0x17 },     /* L_in vol : LR simul-update, unmute, 0dB */
                { 2 | 0x01, 0x17 },     /* R_in vol : LR simul-update, unmute, 0dB */
                { 4 | 0x01, 0xF9 },     /* L_HP vol : LR simul-update, zero-cross, 0dB */
                { 6 | 0x01, 0xF9 },     /* R_HP vol : LR simul-update, zero-cross, 0dB */
                { 8 | 0x00, 0x12 },     /* Analog Audio Path : No Sidetone, No bypass, DAC for Out, Line out for ADC, Mic Mute */
                { 10 | 0x00, 0x00 },    /* Digital Path: DAC unmute, De-emphasis 48k, ADC HPF enable */
                { 12 | 0x00, 0x02 },    /* Power Down : Only Mic is down*/
                { 14 | 0x00, 0x4E },    /* Digital Audio Format : Master, 32bit, I2S */
                { 16 | 0x00, 0x01 },    /* Sanmpling Rate, 48kHz, USB mode*/
//                { 16 | 0x00, 0x00 },    /* Sanmpling Rate, 48kHz, NORMAL mode*/
                { 18 | 0x00, 0x01 }     /* Activateion : Active. */

};


/**
 * @brief コーデックの初期化
 * @details
 * TLV32AIC23Bに初期化数値列をI2C経由で送り込む。タスクコンテキストで呼び出さなければならない。
 * 初期化後、TLV32AIC23Bはクロック信号を生成して動作を開始する。
 */
void init_codec()
{
        int i;


                /* TLV320AIC23Bに初期化データを送信する */
        for ( i=0; i< CODECINITDATALEN; i++){
                i2ccall(i2c_master_write( UZUME_SSM2603_I2C_PORT, UZUME_SSM2603_I2C_ADRS, tlv320aic23init[i], 2 ));
        }
}


