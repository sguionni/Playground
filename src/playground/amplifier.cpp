#include "playground/amplifier.hpp"
#include <imgui/imgui-knobs.h>
#include <string>

namespace Playground
{
	void Amplifier::draw()
	{
		if ( ImGui::TreeNode( ( std::to_string( _id ) + " - " + getName() ).c_str() ) )
		{
			ImGui::PlotLines(
				"", _outputBuffer.data(), int( _outputBuffer.size() ), 0, "Output", -1.0f, 1.0f, ImVec2( 600, 150 ) );
			ImGui::SameLine();
			ImGuiKnobs::Knob( "Volume", &_volume, 0.f, 1.f, 0.005f, "%.2f", ImGuiKnobVariant_Tick );

			ImGui::TreePop();
		}
	}
} // namespace Playground
