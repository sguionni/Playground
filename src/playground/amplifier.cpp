#include "playground/amplifier.hpp"
#include <imgui/imgui-knobs.h>
#include <string>

namespace Playground
{
	void Amplifier::draw()
	{
		if ( ImGui::TreeNode( ( std::to_string( _id ) + " - " + getName() ).c_str() ) )
		{
			ImGui::PlotLines( "", _fOutputBuffer, FRAME_PER_BUFFER, 0, "Output", -1.f, 1.f, ImVec2( 600, 150 ) );
			ImGui::SameLine();
			ImGuiKnobs::Knob( "Volume", &_volume, 0.f, 1.f, 0.005f, "%.2f", ImGuiKnobVariant_Tick );
			ImGui::PlotLines(
				"", _spectrumMagnitudeDB, FRAME_PER_BUFFER, 0, "Spectrum", -6.f, 6.f, ImVec2( 800, 200 ) );
			ImGui::TreePop();
		}
	}
} // namespace Playground
