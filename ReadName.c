/*int printlabel(unsigned char *buffer, int len){
	unsigned char *pbuf;
	int i;
	for(i = 0, pbuf = buffer; i < len;pbuf++, i++){
			//buf[i] = *pbuf;
			printf(":%c",*pbuf);
	}
	//printf(">>>%i\n",i);
	return i;
}*/
int ReadName(unsigned char *reader, unsigned char *buf, int lablen, unsigned char *buffer, int len){
	int pos, _len_ = 0, jump = 0,ptr = 1;
	unsigned char *curpos, *jumped = NULL;
	unsigned char *pbuf = buffer;
	int l = len;
	curpos = reader;
	while((( l>0 || l == -1) && *curpos != 0) || jumped){
		if(jumped){
			if(*jumped >= 0xC0){
				pos = (*jumped << 8|jumped[1])&0x3FFF;
				jumped = &buf[pos];
			}
			_len_ = *jumped++;
			//printlabel(jumped,_len_);
			memcpy(pbuf,jumped,_len_);
			pbuf += _len_;
			*pbuf = '.';
			pbuf++;
			jumped += _len_;
			if(*jumped == 0){
				jumped = NULL;
				break;
			}
		}else{
			if(*curpos>=0xC0){
				pos = ((*curpos&0x3F) << 8)|curpos[1];
				if((buf + pos) > curpos){
					goto read;
				}
				jumped = &buf[pos];
				curpos += 2;
				jump += 2;
				if(l != -1)
					l -= 2;
				ptr = 0;
			}else{
				read:
				_len_ = *curpos++;
				jump++;
				//if(l != -1)
				//	l--;
				//printlabel(curpos,_len_);
				memcpy(pbuf,curpos, _len_);
				pbuf += _len_;
				*pbuf = '.';
				pbuf++;
				curpos += _len_;
				jump += _len_;
				if(l != -1)
					l -= (_len_+1);
			}
		}
		if(lablen > 0 && jump >= lablen){
			*(pbuf - 1) = 0;
			return jump+ptr;
		}
	}
	*(pbuf - 1) = 0;
	return jump+ptr;
}

