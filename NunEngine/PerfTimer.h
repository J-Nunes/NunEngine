#pragma once

class PerfTimer
{
public:

	// Constructor
	PerfTimer();

	void Start();
	double ReadMs() const;
	int ReadTicks() const;

private:
	int	started_at;
	static int frequency;
}; 