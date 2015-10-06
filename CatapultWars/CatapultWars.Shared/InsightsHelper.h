#pragma once

#define INSTRUMENTATION_KEY_PROD L"4e69b654-9cac-49d4-8fa0-361bec35f4b4"
#define INSTRUMENTATION_KEY_DEV L"80da8333-ffaf-4e4c-ba19-1ac20e9141df"

#if defined(_DEBUG)
#define INSTRUMENTATION_KEY INSTRUMENTATION_KEY_DEV
#else
#define INSTRUMENTATION_KEY INSTRUMENTATION_KEY_PROD
#endif

namespace ApplicationInsights {

	static void TrackEvent(Platform::String^ eventName) {
		auto tc = ref new ApplicationInsights::CX::TelemetryClient(INSTRUMENTATION_KEY);
		tc->TrackEvent(eventName);
	}

	static void TrackTrace(Platform::String^ message) {
		auto tc = ref new ApplicationInsights::CX::TelemetryClient(INSTRUMENTATION_KEY);
		tc->TrackTrace(message);
	}

	static void TrackMetric(Platform::String^ name, double value) {
		auto tc = ref new ApplicationInsights::CX::TelemetryClient(INSTRUMENTATION_KEY);
		tc->TrackMetric(name, value);
	}

	static void TrackPageView(Platform::String^ pageName) {
		auto tc = ref new ApplicationInsights::CX::TelemetryClient(INSTRUMENTATION_KEY);
		tc->TrackPageView(pageName);
	}

	static void CloseSession() {
		auto tc = ref new ApplicationInsights::CX::TelemetryClient(INSTRUMENTATION_KEY);
		tc->Flush();
	}

	static void TrackSessionStart() {
		auto tc = ref new ApplicationInsights::CX::TelemetryClient(INSTRUMENTATION_KEY);
		tc->TrackSessionStart();
	}
}