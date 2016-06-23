class Map
{
	private:
		std::vector<Block> map;
		int width, height;
	public:
		void set(std::vector<Block>);
		void setSize(int,int);
		int getWidth();
		int getHeight();
		const Block getBlock(int, int) const;
		const std::vector<Block> get() const;
};
