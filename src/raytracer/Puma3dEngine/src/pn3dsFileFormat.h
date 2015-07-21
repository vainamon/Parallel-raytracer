#ifndef _PN3DSFILEFORMAT_H_
#define _PN3DSFILEFORMAT_H_

//тип - структура заголовка чанка
#pragma pack(2)
typedef struct _3dsChunk{
	//идентификатор чанка
	unsigned short m_usChunkIdentifer;
	//размер чанка
	unsigned int m_ulChunkSize;
} pnS3dsChunk;
#pragma pack()

//идентификаторы чанков
enum {
		CHUNK_VERSION	= 0x0002,
	    CHUNK_RGBF      = 0x0010,
	    CHUNK_RGBB      = 0x0011,

		CHUNK_PERCENTW	= 0x0030,
		CHUNK_PERCENTF	= 0x0031,

	    CHUNK_PRJ       = 0xC23D,
	    CHUNK_MLI       = 0x3DAA,

	    CHUNK_MAIN      = 0x4D4D,
	        CHUNK_OBJMESH   = 0x3D3D,
				CHUNK_ONEUNIT	= 0x0100,
	            CHUNK_BKGCOLOR  = 0x1200,
	            CHUNK_AMBCOLOR  = 0x2100,
				CHUNK_DEFAULT_VIEW = 0x3000,
					CHUNK_VIEW_TOP = 0x3010,
					CHUNK_VIEW_BOTTOM = 0x3020,
					CHUNK_VIEW_LEFT = 0x3030,
					CHUNK_VIEW_RIGHT = 0x3040,
					CHUNK_VIEW_FRONT = 0x3050,
					CHUNK_VIEW_BACK = 0x3060,
					CHUNK_VIEW_USER = 0x3070,
					CHUNK_VIEW_CAMERA = 0x3080,
	            CHUNK_OBJBLOCK  = 0x4000,
	                CHUNK_TRIMESH   = 0x4100,
	                    CHUNK_VERTLIST  = 0x4110,
	                    CHUNK_VERTFLAGS = 0x4111,
	                    CHUNK_FACELIST  = 0x4120,
	                    CHUNK_FACEMAT   = 0x4130,
	                    CHUNK_MAPLIST   = 0x4140,
	                    CHUNK_SMOOLIST  = 0x4150,
	                    CHUNK_TRMATRIX  = 0x4160,
	                    CHUNK_MESHCOLOR = 0x4165,
	                    CHUNK_TXTINFO   = 0x4170,
	                CHUNK_LIGHT     = 0x4600,
	                    CHUNK_SPOTLIGHT = 0x4610,
	                CHUNK_CAMERA    = 0x4700,
	                CHUNK_HIERARCHY = 0x4F00,

	        CHUNK_VIEWPORT_LAYOUT_OLD	= 0x7000,
				CHUNK_VIEWPORT_DATA_OLD	= 0x7010,
					CHUNK_VIEWPORT_SIZE = 0x7020,
						CHUNK_NETWORK_VIEW = 0X7030,

	        CHUNK_VIEWPORT_LAYOUT	= 0x7001,
				CHUNK_VIEWPORT_DATA	= 0x7011,
				CHUNK_VIEWPORT_DATA3 = 0x7012,

			CHUNK_MATERIAL  = 0xAFFF,
	            CHUNK_MATNAME   = 0xA000,
	            CHUNK_AMBIENT   = 0xA010,
	            CHUNK_DIFFUSE   = 0xA020,
	            CHUNK_SPECULAR  = 0xA030,
	            CHUNK_SHININESS = 0xA040,
	            CHUNK_TEXTURE   = 0xA200,
	            CHUNK_BUMPMAP   = 0xA230,
	            CHUNK_MAPFILE   = 0xA300,
	        CHUNK_KEYFRAMER = 0xB000,
	            CHUNK_AMBIENTKEY    = 0xB001,
	            CHUNK_TRACKINFO = 0xB002,
	                CHUNK_TRACKOBJNAME  = 0xB010,
	                CHUNK_TRACKPIVOT    = 0xB013,
	                CHUNK_TRACKPOS      = 0xB020,
	                CHUNK_TRACKROTATE   = 0xB021,
	                CHUNK_TRACKSCALE    = 0xB022,
	                CHUNK_TRACKMORPH    = 0xB026,
	                CHUNK_TRACKHIDE     = 0xB029,
	                CHUNK_OBJNUMBER     = 0xB030,
	            CHUNK_TRACKCAMERA = 0xB003,
	                CHUNK_TRACKFOV  = 0xB023,
	                CHUNK_TRACKROLL = 0xB024,
	            CHUNK_TRACKCAMTGT = 0xB004,
	            CHUNK_TRACKLIGHT  = 0xB005,
	            CHUNK_TRACKLIGTGT = 0xB006,
	            CHUNK_TRACKSPOTL  = 0xB007,
	            CHUNK_FRAMES    = 0xB008
};

#endif //_PN3DSFILEFORMAT_H_
