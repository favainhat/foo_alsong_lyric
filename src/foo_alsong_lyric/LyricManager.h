#pragma once

class LyricManager : public play_callback
{
private:
	string m_Title;
	string m_Album;
	string m_Artist;
	string m_Registrant;
	struct lyricinfo
	{
		lyricinfo(DWORD t, const pfc::string8 &str) : time(t), lyric(str) {}
		lyricinfo() : time(0), lyric("") {}
		DWORD time;
		pfc::string8 lyric;
	};
	vector<lyricinfo> m_Lyric;
	int m_Lyricpos;
	int m_Seconds;

	boost::signals2::signal<void ()> RedrawHandler;
	boost::shared_ptr<boost::thread> m_fetchthread;
	boost::shared_ptr<boost::thread> m_countthread;
	boost::posix_time::ptime begin;
	boost::posix_time::ptime tick;

	static DWORD GetFileHash(metadb_handle_ptr track, CHAR *Hash);
	DWORD ParseLyric(const char *InputLyric, const char *Delimiter);
	void CountLyric(const metadb_handle_ptr &track);
	DWORD DownloadLyric(CHAR *Hash);
	DWORD FetchLyric(const metadb_handle_ptr &track);
	void Clear();
	
	static SOCKET InitateConnect(CHAR *Address, int port);
public:
	LyricManager();
	~LyricManager();

	static DWORD UploadLyric(metadb_handle_ptr track, int PlayTime, int nInfo, int UploadType, 
		pfc::string8 Lyric, pfc::string8 Title, pfc::string8 Artist, pfc::string8 Album, pfc::string8 Registrant);
	static DWORD SearchLyricGetNext(CHAR **data, int &Info, pfc::string8 Title, pfc::string8 Artist, pfc::string8 Album, pfc::string8 Lyric, pfc::string8 Registrant);
	static int SearchLyricGetCount(const pfc::string8 &Artist, const pfc::string8 &Title);
	static DWORD SearchLyric(const pfc::string8 &Artist, const pfc::string8 Title, int nPage, CHAR **Output);
	
	boost::signals2::connection AddRedrawHandler(const boost::signals2::signal<void ()>::slot_type &Handler)
	{
		return RedrawHandler.connect(Handler);
	}

	std::vector<pfc::string8> GetLyricBefore(int n); //이전 가사. n:줄수
	std::vector<pfc::string8> GetLyric(); //현재 표시할 가사 보여주기
	std::vector<pfc::string8> GetLyricAfter(int n); //다음가사. n:줄수

	void SaveToFile(WCHAR *SaveTo, CHAR *fmt);
	DWORD LoadFromFile(WCHAR *LoadFrom, CHAR *fmt);

	// play_callback methods (the ones we're interested in)
	virtual void on_playback_seek(double p_time);
	virtual void on_playback_new_track(metadb_handle_ptr p_track);
	virtual void on_playback_stop(play_control::t_stop_reason reason);
	virtual void on_playback_time(double p_time);
	virtual void on_playback_pause(bool p_state);

	// play_callback methods (the rest)
	virtual void on_playback_dynamic_info_track(const file_info & p_info) {}
	virtual void on_playback_starting(play_control::t_track_command p_command, bool p_paused) {}
	virtual void on_playback_edited(metadb_handle_ptr p_track) {}
	virtual void on_playback_dynamic_info(const file_info & p_info) {}
	virtual void on_volume_change(float p_new_val) {}
};

extern LyricManager *LyricManagerInstance; //singleton. must created on on_init

void md5( unsigned char *input, int ilen, unsigned char output[16] );
