#include "includes.h"

bool Hooks::ShouldDrawParticles( ) {
	return g_hooks.m_client_mode.GetOldMethod< ShouldDrawParticles_t >( IClientMode::SHOULDDRAWPARTICLES )( this );
}

bool Hooks::ShouldDrawFog( ) {
	// remove fog.
	return g_hooks.m_client_mode.GetOldMethod< ShouldDrawFog_t >( IClientMode::SHOULDDRAWFOG )( this );
}

void Hooks::OverrideView( CViewSetup* view ) {
	// damn son.
	g_cl.m_local = g_csgo.m_entlist->GetClientEntity< Player* >( g_csgo.m_engine->GetLocalPlayer( ) );

	// g_grenades.think( );
	g_visuals.ThirdpersonThink( );

    // call original.
	g_hooks.m_client_mode.GetOldMethod< OverrideView_t >( IClientMode::OVERRIDEVIEW )( this, view );

    // remove scope edge blur.
	if (g_menu.main.visuals.removals.get(4)) {
		if( g_cl.m_local && g_cl.m_local->m_bIsScoped( ) )
            view->m_edge_blur = 0;
	}
}

/*bool Hooks::CreateMove( float time, CUserCmd* cmd ) {
	Stack   stack;
	bool    ret;

	// let original run first.
	ret = g_hooks.m_client_mode.GetOldMethod< CreateMove_t >( IClientMode::CREATEMOVE )( this, time, cmd );

	// called from CInput::ExtraMouseSample -> return original.
	if( !cmd->m_command_number )
		return ret;

	// if we arrived here, called from -> CInput::CreateMove
	// call EngineClient::SetViewAngles according to what the original returns.
	if( ret )
		g_csgo.m_engine->SetViewAngles( cmd->m_view_angles );

	// random_seed isn't generated in ClientMode::CreateMove yet, we must set generate it ourselves.
	cmd->m_random_seed = g_csgo.MD5_PseudoRandom( cmd->m_command_number ) & 0x7fffffff;

	// get bSendPacket off the stack.
	g_cl.m_packet = stack.next( ).local( 0x1c ).as< bool* >( );

	// get bFinalTick off the stack.
	g_cl.m_final_packet = stack.next( ).local( 0x1b ).as< bool* >( );

	// invoke move function.
	g_cl.OnTick( cmd );

	return false;
}*/

//void draw_beam(vec3_t start, vec3_t end, Color color, float Width) {
//	BeamInfo_t info;
//
//	info.m_nType = 0;
//	info.m_pszModelName = "sprites/purplelaser1.vmt";
//	info.m_nModelIndex = -1;
//	info.m_flHaloScale = 0.f;
//	info.m_flLife = 2.5f;
//	info.m_flWidth = Width;
//	info.m_flEndWidth = 1;
//	info.m_flFadeLength = 0.f;
//	info.m_flAmplitude = 2.f;
//	info.m_flBrightness = float(color.a());
//	info.m_flSpeed = .2f;
//	info.m_nStartFrame = 0;
//	info.m_flFrameRate = 0.f;
//	info.m_flRed = float(color.r());
//	info.m_flGreen = float(color.g());
//	info.m_flBlue = float(color.b());
//	info.m_nSegments = 2;
//	info.m_bRenderable = true;
//	info.m_nFlags = 0;
//	info.m_vecStart = start;
//	info.m_vecEnd = end;
//
//	auto myBeam = g_csgo.m_beams->CreateBeamPoints(info);
//	if (myBeam)
//		g_csgo.m_beams->DrawBeam(myBeam);
//}

bool Hooks::CreateMove(float time, CUserCmd* cmd) {
	Stack   stack;
	bool    ret;
	
	////Trails
	//auto local = g_cl.m_local;
	//static vec3_t old_origin = local->m_vecOrigin();
	//auto origin = local->m_vecOrigin() + vec3_t(0, 0, 5);
	//auto start = old_origin, end = origin;
	//Color color = Color(255);
	//constexpr auto width = 1.6f;
	//if (old_origin.length_2d() != origin.length_2d())
	//	draw_beam(start, end, color, width);

	//old_origin = origin;

	ret = g_hooks.m_client_mode.GetOldMethod< CreateMove_t >(IClientMode::CREATEMOVE)(this, time, cmd);
	// called from CInput::ExtraMouseSample -> return original.
	if (!cmd || !cmd->m_command_number)
		return g_hooks.m_client_mode.GetOldMethod< CreateMove_t >(IClientMode::CREATEMOVE)(this, time, cmd);

	// if we arrived here, called from -> CInput::CreateMove
	// call EngineClient::SetViewAngles according to what the original returns.
	if (g_hooks.m_client_mode.GetOldMethod< CreateMove_t >(IClientMode::CREATEMOVE)(this, time, cmd))
		g_csgo.m_engine->SetViewAngles(cmd->m_view_angles);

	// random_seed isn't generated in ClientMode::CreateMove yet, we must set generate it ourselves.
	cmd->m_random_seed = g_csgo.MD5_PseudoRandom(cmd->m_command_number) & 0x7fffffff;

	// get bSendPacket off the stack.
	g_cl.m_packet = stack.next().local(0x1c).as< bool* >();

	// get bFinalTick off the stack.
	g_cl.m_final_packet = stack.next().local(0x1b).as< bool* >();

	// invoke move function.

	g_cl.OnTick(cmd);

	return false;
}

bool Hooks::DoPostScreenSpaceEffects( CViewSetup* setup ) {
	g_visuals.RenderGlow( );

	return g_hooks.m_client_mode.GetOldMethod< DoPostScreenSpaceEffects_t >( IClientMode::DOPOSTSPACESCREENEFFECTS )( this, setup );
}