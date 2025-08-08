///////////////////////////////////////////////////////////////////////////////
// 雑多な定義

#ifndef __MISC_H__
#define __MISC_H__

  ////////////////////////////////////////////////////////////////////////////
  // 汎用マクロ
  #ifndef ABS
		#define ABS( val )					      	( ((val) > 0 ) ? (val) : -(val) )
  #endif
  #ifndef MIN
		#define MIN( one, other )			    	( (one) < (other) ? (one) : (other) )
  #endif
  #ifndef MAX
		#define MAX( one, other )			    	( (one) > (other) ? (one) : (other) )
  #endif
  #define MAX3(x,y,z)					      		( MAX(MAX(x,y),z) )
  #define MIN3(x,y,z)					      		( MIN(MIN(x,y),z) )
  #define MED3(x,y,z)					      		( (x)+(y)+(z)-MAX3(x,y,z)-MIN3(x,y,z) )
  #define NNEG( val )					      		( ( (val) > 0     ) ? (val)  : 0 )
  #define NPOS( val )					      		( ( (val) < 0     ) ? (val)  : 0 )
  #define NVAL( val )					      		( ( (val) < 0     ) ? -(val) : 0 )
  #define PVAL( val )					      		( ( (val) > 0     ) ? (val)  : 0 )
  #define SATURATE( val, max )		  		( ( (val) < (max) ) ? (val) : (max) )
  #define RESTRICT( val, min, max )			( ( (val) < (max) ) ? ( ( (val) > (min) ) ? (val) : (min) ) : (max) )
  #define ROUND( val )				      		( (val) + ( (val) > 0.0 ? 0.5 : -0.5 ) )
  #define SQUARE( val )				      		( (val)*(val) )
  #define CUBE( val )					      		( (val)*(val)*(val) )
  #define SIGN( val )					      		( ((val) < 0 ) ? -1 : 1 )
  #define ISEVN( val )				      		( ((val) & 1 ) == 0 )
  #define ISODD( val )				      		( ((val) & 1 ) != 0 )
  #define TOGLE( tgt, min, max )				( (tgt) = ( (tgt) < (max) ? ((tgt)+1) : (min) ) )
  #define ARRAYSZ( array )	        		(sizeof(array)/sizeof((array)[0]))
  #define MEMCPY( dst, src, type, cnt )	( memcpy( (void*)(dst), (void*)(src), sizeof(type)*(cnt) ) )
  #define MEMSET( buf, ch,  type, cnt )	( memset( (void*)(buf), (int  )(ch),  sizeof(type)*(cnt) ) )

	////////////////////////////////////////////////////////////////////////////
	//! @brief リングバッファの初期化
	//! @param	rngtyp	バッファ型
	//! @param	rngbuff	バッファ
	//! @param	rngstt	開始位置
	//! @param	rngend	終了位置
	//! @param	rngcnt	データ数
	//! @param	rngnum	バッファサイズ
	#define RING_INIT( rngtyp, rngbuff, rngstt, rngend, rngcnt, rngnum ) \
	{                                                                    \
		(rngstt) = 0;                                                      \
		(rngend) = 0;                                                      \
		(rngcnt) = 0;                                                      \
	}

	////////////////////////////////////////////////////////////////////////////
	//! @brief カウント取得
	//! @param	rngtyp	バッファ型
	//! @param	rngbuff	バッファ
	//! @param	rngstt	開始位置
	//! @param	rngend	終了位置
	//! @param	rngcnt	データ数
	//! @param	rngnum	バッファサイズ
	#define RING_COUNT( rngtyp, rngbuff, rngstt, rngend, rngcnt, rngnum ) \
		(rngcnt)

	////////////////////////////////////////////////////////////////////////////
	//! インデックス/カウント変更(内部用)
	#define RING_IDX_INC( rngpos, rngcnt, rngnum, datcnt ) \
	{                                                      \
		(rngpos) = (((rngpos)+(datcnt)) & ((rngnum)-1));     \
	}
	#define RING_CNT_INC( rngpos, rngcnt, rngnum, datcnt ) \
	{                                                      \
		(rngcnt) = (((rngcnt)+(datcnt)));                    \
	}
	#define RING_CNT_DEC( rngpos, rngcnt, rngnum, datcnt ) \
	{                                                      \
		(rngcnt) = (((rngcnt)-(datcnt)));                    \
	}

	////////////////////////////////////////////////////////////////////////////
	//! @brief 追加可能判定
	//! @param	rngtyp	バッファ型
	//! @param	rngbuff	バッファ
	//! @param	rngstt	開始位置
	//! @param	rngend	終了位置
	//! @param	rngcnt	データ数
	//! @param	rngnum	バッファサイズ
	//! @param	datacnt	追加数
	#define RING_CAN_ENQUEUE( rngtyp, rngbuff, rngstt, rngend, rngcnt, rngnum, datacnt ) \
		( ( (rngnum) - (rngcnt) >= (datacnt) ) ? (1) : (0) )

	////////////////////////////////////////////////////////////////////////////
	//! @brief 単体追加
	//! @param	rngtyp	バッファ型
	//! @param	rngbuff	バッファ
	//! @param	rngstt	開始位置
	//! @param	rngend	終了位置
	//! @param	rngcnt	データ数
	//! @param	rngnum	バッファサイズ
	//! @param	data	追加データ
	#define RING_ENQUEUE( rngtyp, rngbuff, rngstt, rngend, rngcnt, rngnum, data ) \
	{                                                                             \
		((rngbuff)[(rngend)]) = data;                                               \
		RING_IDX_INC( (rngend), (rngcnt), (rngnum), 1 );                            \
		RING_CNT_INC( (rngend), (rngcnt), (rngnum), 1 );                            \
	}

	////////////////////////////////////////////////////////////////////////////
	//! @brief 複数追加
	//! @param	rngtyp	バッファ型
	//! @param	rngbuff	バッファ
	//! @param	rngstt	開始位置
	//! @param	rngend	終了位置
	//! @param	rngcnt	データ数
	//! @param	rngnum	バッファサイズ
	//! @param	data	追加データ
	//! @param	datacnt	追加データ数
	#define RING_ENQUEUE_RANGE( rngtyp, rngbuff, rngstt, rngend, rngcnt, rngnum, data, datacnt )                                                   \
	{                                                                                                                                              \
		if( (rngnum) > (rngend) + (datacnt) ){                                                                                                       \
			memcpy( (void*)(&((rngbuff)[(rngend)])), (void*)(&((data)[                  0])), ((datacnt)                      ) * sizeof(rngtyp));     \
		}else{                                                                                                                                       \
			memcpy( (void*)(&((rngbuff)[(rngend)])), (void*)&(((data)[                  0])), (            ((rngnum)-(rngend))) * sizeof(rngtyp));     \
			memcpy( (void*)(&((rngbuff)[       0])), (void*)&(((data)[((rngnum)-(rngend))])), ((datacnt) - ((rngnum)-(rngend))) * sizeof(rngtyp));     \
		}                                                                                                                                            \
		RING_IDX_INC( (rngend), (rngcnt), (rngnum), (datacnt) );                                                                                     \
		RING_CNT_INC( (rngend), (rngcnt), (rngnum), (datacnt) );                                                                                     \
	}

	////////////////////////////////////////////////////////////////////////////
	//! @brief 取得可能判定
	//! @param	rngtyp	バッファ型
	//! @param	rngbuff	バッファ
	//! @param	rngstt	開始位置
	//! @param	rngend	終了位置
	//! @param	rngcnt	データ数
	//! @param	rngnum	バッファサイズ
	//! @param	datacnt	取得数
	#define RING_CAN_DEQUEUE( rngtyp, rngbuff, rngstt, rngend, rngcnt, rngnum, datacnt ) \
		( ( (rngcnt) >= (datacnt) ) ? (1) : (0) )

	////////////////////////////////////////////////////////////////////////////
	//! @brief 単体取得
	//! @param	rngtyp	バッファ型
	//! @param	rngbuff	バッファ
	//! @param	rngstt	開始位置
	//! @param	rngend	終了位置
	//! @param	rngnum	バッファサイズ
	//! @param	data	取得データ
	#define RING_DEQUEUE( rngtyp, rngbuff, rngstt, rngend, rngcnt, rngnum, data ) \
	{                                                                             \
		data = ((rngbuff)[(rngstt)]);                                               \
		RING_IDX_INC( (rngstt), (rngcnt), (rngnum), 1 );                            \
		RING_CNT_DEC( (rngstt), (rngcnt), (rngnum), 1 );                            \
	}

	////////////////////////////////////////////////////////////////////////////
	//! @brief 複数取得
	//! @param	rngtyp	バッファ型
	//! @param	rngbuff	バッファ
	//! @param	rngstt	開始位置
	//! @param	rngend	終了位置
	//! @param	rngcnt	データ数
	//! @param	rngnum	バッファサイズ
	//! @param	data	取得データ
	//! @param	datacnt	取得データ数
	//! @param	fdisirq	割込み禁止開始処理
	//! @param	fenairq	割込み禁止終了処理
	#define RING_DEQUEUE_RANGE( rngtyp, rngbuff, rngstt, rngend, rngcnt, rngnum, data, datacnt )                                                  \
	{                                                                                                                                             \
		if( (rngnum) > (rngstt) + (datacnt) ){                                                                                                      \
			memcpy( (void*)(&((data)[                  0])), (void*)(&((rngbuff)[(rngstt)])), ((datacnt)                      ) * sizeof(rngtyp) );   \
		}else{                                                                                                                                      \
			memcpy( (void*)(&((data)[                  0])), (void*)(&((rngbuff)[(rngstt)])), (            ((rngnum)-(rngstt))) * sizeof(rngtyp) );   \
			memcpy( (void*)(&((data)[((rngnum)-(rngstt))])), (void*)(&((rngbuff)[       0])), ((datacnt) - ((rngnum)-(rngstt))) * sizeof(rngtyp) );   \
		}                                                                                                                                           \
		RING_IDX_INC( (rngstt), (rngcnt), (rngnum), (datacnt) );                                                                                    \
		RING_CNT_DEC( (rngstt), (rngcnt), (rngnum), (datacnt) );                                                                                    \
	}

	////////////////////////////////////////////////////////////////////////////
	//! @brief ピーク
	//! @param	rngtyp	バッファ型
	//! @param	rngbuff	バッファ
	//! @param	rngstt	開始位置
	//! @param	rngend	終了位置
	//! @param	rngcnt	データ数
	//! @param	rngnum	バッファサイズ
	#define RING_PEEK( rngtyp, rngbuff, rngstt, rngend, rngcnt, rngnum ) \
		((rngbuff)[(rngstt)])

	////////////////////////////////////////////////////////////////////////////
	//! @brief スキップ
	//! @param	rngtyp	バッファ型
	//! @param	rngbuff	バッファ
	//! @param	rngstt	開始位置
	//! @param	rngend	終了位置
	//! @param	rngcnt	データ数
	//! @param	rngnum	バッファサイズ
	//! @param	datacnt	スキップ数
	#define RING_SKIP( rngtyp, rngbuff, rngstt, rngend, rngcnt, rngnum, datacnt ) \
	{                                                                             \
		RING_IDX_INC( (rngstt), (rngcnt), (rngnum), (datacnt) );                    \
		RING_CNT_DEC( (rngstt), (rngcnt), (rngnum), (datacnt) );                    \
	}

#endif
