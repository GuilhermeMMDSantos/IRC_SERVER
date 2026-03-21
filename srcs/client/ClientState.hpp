#ifndef __CLIENTSTATE_HPP__
#define __CLIENTSTATE_HPP__

class ClientState
{
	private:
		bool	isPassWordOk;
		bool	isNickNameOk;
		bool	isUserNameOk;

	public:
		ClientState(bool p = false, bool n = false, bool u = false);
};

#endif
