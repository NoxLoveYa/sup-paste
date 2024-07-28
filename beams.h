#pragma once

#define MAX_BEAM_ENTS	10
#define NOISE_DIVISIONS	128

//class Beam_t {
//public:
//    PAD( 52 );       // 0x0
//	int flags;       // 0x34
//	PAD( 144 );      
//	float die;       // 0xC8
//	PAD( 20 );       
//	float r;         // 0xE0
//	float g;         // 0xE4
//	float b;         // 0xE8
//	PAD( 8 );
//	float frameRate; // 0xF4
//	float frame;     // 0xF8
//}; 

class Beam_t {
public:
    void    *unk_1; // probably 2-3 vmts?
    void    *unk_2;
    void    *unk_3;
	vec3_t	m_Mins;
	vec3_t	m_Maxs;
	void    *m_queryHandleHalo;
	float	m_haloProxySize;
	Beam_t  *next;
	int		type;
	int		flags;
	int		numAttachments;
	vec3_t	attachment[ MAX_BEAM_ENTS ];
	vec3_t	delta;
	float	t;		
	float	freq;
	float	die;
	float	width;
	float	endWidth;
	float	fadeLength;
	float	amplitude;
	float	life;
	float	r, g, b;
	float	brightness;
	float	speed;
	float	frameRate;
	float	frame;
	int		segments;
	EHANDLE	entity[ MAX_BEAM_ENTS ];
	int		attachmentIndex[ MAX_BEAM_ENTS ];
	int		modelIndex;
	int		haloIndex;
	float	haloScale;
	int		frameCount;
	float	rgNoise[ NOISE_DIVISIONS + 1 ];
	void    *trail;
	float	start_radius;
	float	end_radius;
	bool	m_bCalculatedNoise;
	float	m_flHDRColorScale;
};

struct BeamInfo_t {
    int		   m_nType;
	Entity     *m_pStartEnt;
	int		   m_nStartAttachment;
	Entity     *m_pEndEnt;
	int		   m_nEndAttachment;
	vec3_t	   m_vecStart;
	vec3_t	   m_vecEnd;
	int		   m_nModelIndex;
	const char *m_pszModelName;
	int		   m_nHaloIndex;
	const char *m_pszHaloName;
	float	   m_flHaloScale;
	float	   m_flLife;
	float	   m_flWidth;
	float	   m_flEndWidth;
	float	   m_flFadeLength;
	float	   m_flAmplitude;
	float	   m_flBrightness;
	float	   m_flSpeed;
	int		   m_nStartFrame;
	float	   m_flFrameRate;
	float	   m_flRed;
	float	   m_flGreen;
	float	   m_flBlue;
	bool	   m_bRenderable;
	int		   m_nSegments;
	int		   m_nFlags;
	vec3_t	   m_vecCenter;
	float	   m_flStartRadius;
	float	   m_flEndRadius;

	__forceinline BeamInfo_t() { 
		m_nType        = 0; // TE_BEAMPOINTS;
		m_nSegments    = -1;
		m_pszModelName = 0;
		m_pszHaloName  = 0;
		m_nModelIndex  = -1;
		m_nHaloIndex   = -1;
		m_bRenderable  = true;
		m_nFlags       = 0;
	}
};

class IViewRenderBeams {
public:
	enum indices : size_t {
        DRAWBEAM         = 4,
        CREATEBEAMPOINTS = 12,
        UPDATEBEAMINFO   = 22
	};

    __forceinline void DrawBeam( Beam_t *beam ) {
        util::get_method< void (__thiscall *)( void *, Beam_t * ) >( this, DRAWBEAM )( this, beam );
    }

    __forceinline Beam_t *CreateBeamPoints( BeamInfo_t &beam_info ) {
        return util::get_method< Beam_t *(__thiscall *)( void *, BeamInfo_t & ) >( this, CREATEBEAMPOINTS )( this, beam_info );
    }

    __forceinline void UpdateBeamInfo( Beam_t *beam, BeamInfo_t &beam_info ) {
        util::get_method< void (__thiscall *)( void *, Beam_t *, BeamInfo_t & ) >( this, UPDATEBEAMINFO )( this, beam, beam_info );
    }
};

enum dlight_flags {
	dlight_no_world_illumination = 0x1,
	dlight_no_model_illumination = 0x2,
	dlight_add_displacement_alpha = 0x4,
	dlight_subtract_displacement_alpha = 0x8,
	dlight_displacement_mask = (dlight_add_displacement_alpha | dlight_subtract_displacement_alpha),
};

struct ColorRGBExp32
{
	byte r, g, b;
	signed char exponent;
};

struct Dlight_t {
	int flags;
	vec3_t origin;
	float radius;
	ColorRGBExp32 color;
	float die_time;
	float decay;
	float min_light;
	int	key;
	int	style;
	vec3_t direction;
	float inner_angle;
	float outer_angle;
};

class IVEffects
{
public:
	Dlight_t* cl_alloc_dlight(int key) {
		using original_fn = Dlight_t * (__thiscall*)(void*, int);
		return (*(original_fn**)this)[4](this, key);
	}
	Dlight_t* cl_alloc_elight(int key) {
		using original_fn = Dlight_t * (__thiscall*)(void*, int);
		return (*(original_fn**)this)[5](this, key);
	}
	Dlight_t* get_elight_by_key(int key) {
		using original_fn = Dlight_t * (__thiscall*)(void*, int);
		return (*(original_fn**)this)[8](this, key);
	}
};
