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
		try {
			auto tc = ref new ApplicationInsights::CX::TelemetryClient(INSTRUMENTATION_KEY);
			tc->TrackEvent(eventName);
		}
		catch (...) {

		}
	}

	static void TrackTrace(Platform::String^ message) {
		try {
			auto tc = ref new ApplicationInsights::CX::TelemetryClient(INSTRUMENTATION_KEY);
			tc->TrackTrace(message);
		}
		catch (...) {

		}
	}

	static void TrackMetric(Platform::String^ name, double value) {
		try {
			auto tc = ref new ApplicationInsights::CX::TelemetryClient(INSTRUMENTATION_KEY);
			tc->TrackMetric(name, value);
		}
		catch (...) {

		}
	}

	static void TrackPageView(Platform::String^ pageName) {
		try {
			auto tc = ref new ApplicationInsights::CX::TelemetryClient(INSTRUMENTATION_KEY);
			tc->TrackPageView(pageName);
		}
		catch (...) {

		}
	}

	static void CloseSession() {
		try {
			auto tc = ref new ApplicationInsights::CX::TelemetryClient(INSTRUMENTATION_KEY);
			tc->Flush();
		}
		catch (...) {

		}
	}

	static void TrackSessionStart() {
		try {
			auto tc = ref new ApplicationInsights::CX::TelemetryClient(INSTRUMENTATION_KEY);
			tc->TrackSessionStart();
		}
		catch (...) {

		}
	}
}